/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "schema.hpp"

#include "debug.hpp"
#include "maths.hpp"
#include "./tree.hpp"
#include "./except.hpp"

#include <regex>


///////////////////////////////////////////////////////////////////////////////
static bool validate (json::tree::node&, const json::tree::object&);


///////////////////////////////////////////////////////////////////////////////
static bool
validate (json::tree::object &node,
          const json::tree::object &schema)
{
    bool success = true;

    auto properties = schema.find ("properties");
    auto additional = schema.find ("additionalProperties");
    auto pattern    = schema.find ("patternProperties");

    if (properties != schema.cend ()) {
        for (const auto &kv: properties->second->as_object ()) {
            auto p = node.find (kv.first);
            if (p != node.cend ())
                success = success && validate (*p->second, kv.second->as_object ());
            else {
                try {
                    node.insert (kv.first, (*kv.second)["default"].clone ());
                    success = success && validate (node[kv.first], kv.second->as_object ());
                } catch (const json::key_error&)
                { ; }
            }
        }
    }

    if (pattern != schema.cend ())
        not_implemented ();

    if (additional != schema.cend ())
        not_implemented ();

    if (schema.has ("dependencies"))
        not_implemented ();

    // properties must be checked after the 'properties' check has a chance to
    // create the defaulted entries.
    auto maxProperties = schema.find ("maxProperties");
    if (maxProperties != schema.cend ())
        success = success && node.size () <= maxProperties->second->as_uint ();

    auto minProperties = schema.find ("minProperties");
    if (minProperties != schema.cend ())
        success = success && node.size () >= minProperties->second->as_uint ();

    auto required = schema.find ("required");
    if (required != schema.cend ())
        for (const auto &i: required->second->as_array ())
            success = success && node.has (i.as_string ());

    return success;
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::array &node,
          const json::tree::object &schema)
{
    bool success = true;

    // attempt to match the item and additionalItem schemas
    auto items = schema.find ("items");
    auto additional = schema.find ("additionalItems");

    if (items != schema.cend ()) {
        // items is an object, test all elements with it as a schema
        if (items->second->is_object ()) {
            for (auto &i: node)
                success = success && validate (i, items->second->as_object ());
        // items is a list of schemas, test n-elements with it as a schema
        } else if (items->second->is_array ()) {
            const auto &itemArray = items->second->as_array ();

            size_t i = 0;
            for (; i < itemArray.size () && i < node.size (); ++i)
                success = success && validate (node[i], itemArray[i].as_object ());

            // we've exhausted the schema list, use the additional schema
            if (i == itemArray.size ()) {
                if (additional->second->is_boolean ()) {
                    success = success && additional->second->as_boolean ();
                } else if (additional->second->is_object ()) {
                    for ( ; i < node.size (); ++i)
                        success = success && validate (node[i], additional->second->as_object ());
                } else {
                    success = false;
                }
            }
        }
    }

    auto maxItems = schema.find ("maxItems");
    if (maxItems != schema.cend ())
        success = success && node.size () <= maxItems->second->as_uint ();

    auto minItems = schema.find ("minItems");
    if (minItems != schema.cend ())
        success = success && node.size () >= minItems->second->as_uint ();

    // check all element are unique
    // XXX: uses a naive n^2 brute force search on equality because it's 2am
    // and I don't want to write a type aware comparator for the sort.
    auto unique = schema.find ("uniqueItems");
    if (unique != schema.cend () && unique->second->as_boolean ()) {
        for (size_t a = 0; a < node.size (); ++a)
            for (size_t b = a + 1; b < node.size (); ++b)
                if (node[a] == node[b]) {
                    success = false;
                    goto notunique;
                }
notunique: ;
    }

    return success;
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::string &node,
          const json::tree::object &schema)
{
    const auto &val = node.native ();

    // check length is less than a maximum
    auto maxLength = schema.find ("maxLength");
    if (maxLength != schema.cend ()) {
        auto cmp = maxLength->second->as_number ().native ();
        if (!is_integer (cmp))
            return false;

        if (val.size () > cmp)
            return false;
    }

    // check length is greater than a maximum
    auto minLength = schema.find ("minLength");
    if (minLength != schema.cend ()) {
        auto cmp = minLength->second->as_number ().native ();
        if (!is_integer (cmp))
            return false;

        if (val.size () < cmp)
            return false;
    }

    // check the string conforms to a regex
    // Note: this uses the c++11 regex engine which slightly differs from ECMA 262
    auto pattern = schema.find ("pattern");
    if (pattern != schema.cend ()) {
        std::regex r (pattern->second->as_string ().native (),
                      std::regex_constants::ECMAScript);
        if (!std::regex_search (val, r))
            return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::number &node,
          const json::tree::object &schema)
{
    const auto &val = node.native ();

    // check strictly positive integer multiple
    auto mult = schema.find ("multipleOf");
    if (mult != schema.cend ()) {
        auto div = mult->second->as_number ().native ();

        if (val <= 0 || almost_equal (val, div))
            return false;
    }

    // check maximum holds. exclusive requires max condition.
    auto max = schema.find ("maximum");
    auto exclusiveMax = schema.find ("exclusiveMaximum");
    if (max != schema.cend ()) {
        auto cmp = max->second->as_number ().native ();

        if (exclusiveMax->second->as_boolean () ? (val <= cmp) : (val < cmp))
            return false;
    } else {
        if (exclusiveMax != schema.cend ())
            return false;
    }

    // check minimum holds. exclusive requires min condition
    auto min = schema.find ("minimum");
    auto exclusiveMin = schema.find ("exclusiveMinimum");
    if (min != schema.cend ()) {
        auto cmp = min->second->as_number ().native ();

        if (exclusiveMin->second->as_boolean () ? val >= cmp : val > cmp)
            return false;
    } else {
        if (exclusiveMin != schema.cend ())
            return false;
    }

    return true;
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::boolean&,
          const json::tree::object&)
{
    return true;
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::null&,
          const json::tree::object&)
{
    return true;
}


//-----------------------------------------------------------------------------
static std::string
to_string (json::tree::type_t t)
{
    switch (t) {
        case json::tree::OBJECT:    return "object";
        case json::tree::ARRAY:     return "array";
        case json::tree::STRING:    return "string";
        case json::tree::NUMBER:    return "number";
        case json::tree::BOOLEAN:   return "boolean";
        case json::tree::NONE:      return "null";
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static bool
validate (json::tree::node &node,
          const json::tree::object &schema)
{
    // check the value is in the prescribed list
    auto enumPos = schema.find ("enum");
    if (enumPos != schema.cend ()) {
        auto pos = std::find (enumPos->second->as_array ().cbegin (),
                              enumPos->second->as_array ().cend (),
                              node);
        if (pos == enumPos->second->as_array ().cend ())
            return false;
    }

    auto type = schema.find ("type");
    if (type != schema.cend ()) {
        if (type->second->is_string ()) {
            auto a = type->second->as_string ();
            auto b = to_string (node.type ());

            if (a != b) {
                std::cerr << a << " != " << b << '\n';
                return false;
            }
        } else if (type->second->is_array ()) {
            auto pos = std::find_if (type->second->as_array ().begin (),
                                     type->second->as_array ().end (),
                                     [&] (const auto &i) { return i.as_string () == to_string (node.type ()); });
            if (pos == type->second->as_array ().end ())
                return false;
        } else
            return false;
    }

    auto allOf = schema.find ("allOf");
    if (allOf != schema.cend ()) {
        for (const auto &i: allOf->second->as_array ())
            if (!validate (node, i.as_object ()))
                return false;
    }

    auto anyOf = schema.find ("anyOf");
    if (anyOf != schema.cend ()) {
        bool success = false;
        for (const auto &i: anyOf->second->as_array ()) {
            success = validate (node, i.as_object ());
            if (success)
                break;
        }

        if (!success)
            return false;
    }

    auto oneOf = schema.find ("oneOf");
    if (oneOf != schema.cend ()) {
        unsigned count = 0;

        for (const auto &i: oneOf->second->as_array ()) {
            if (validate (node, i.as_object ()))
                count++;
            if (count > 1)
                return false;
        }

        if (count != 1)
            return false;
    }

    auto notSchema = schema.find ("not");
    if (notSchema != schema.cend ()) {
        for (const auto &i: notSchema->second->as_array ())
            if (validate (node, i.as_object ()))
                return false;
    }

    switch (node.type ()) {
        case json::tree::OBJECT:    return validate (node.as_object (),     schema);
        case json::tree::ARRAY:     return validate (node.as_array (),      schema);
        case json::tree::STRING:    return validate (node.as_string (),     schema);
        case json::tree::NUMBER:    return validate (node.as_number (),     schema);
        case json::tree::BOOLEAN:   return validate (node.as_boolean (),    schema);
        case json::tree::NONE:      return validate (node.as_null (),       schema);
    }

    unreachable ();
    return false;
}


//-----------------------------------------------------------------------------
bool
json::schema::validate (json::tree::node &data,
                        const json::tree::object &schema)
{
    auto title = schema.find ("title");
    if (title != schema.cend ())
        if (!title->second->is_string ())
            return false;

    auto description = schema.find ("description");
    if (description != schema.cend ())
        if (!description->second->is_string ())
            return false;

    return ::validate (data, schema.as_object ());
}
