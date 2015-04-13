/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
struct length_error : public json::schema_error {
    using schema_error::schema_error;
};


struct format_error : public json::schema_error {
    using schema_error::schema_error;
};


///////////////////////////////////////////////////////////////////////////////
static void validate (json::tree::node&, const json::tree::object&);


///////////////////////////////////////////////////////////////////////////////
static void
validate (json::tree::object &node,
          const json::tree::object &schema)
{
    auto properties = schema.find ("properties");
    auto additional = schema.find ("additionalProperties");
    auto pattern    = schema.find ("patternProperties");

    if (properties != schema.cend ()) {
        for (const auto &kv: properties->second->as_object ()) {
            auto p = node.find (kv.first);
            if (p != node.cend ())
                validate (*p->second, kv.second->as_object ());
            else {
                try {
                    node.insert (kv.first, (*kv.second)["default"].clone ());
                    validate (node[kv.first], kv.second->as_object ());
                    continue;
                } catch (const json::key_error&)
                { ; }

                if (additional != schema.cend ()) {
                    if (additional->second->is_boolean () && !additional->second->as_bool ())
                        throw json::schema_error ("additionalProperties");

                    validate (*p->second, additional->second->as_object ());
                }
            }
        }
    }

    if (pattern != schema.cend ()) {
        for (auto &cond: pattern->second->as_object ()) {
            std::regex expr (cond.first, std::regex_constants::ECMAScript);

            for (auto &props: node) {
                if (std::regex_search (props.first, expr))
                    validate (*props.second, cond.second->as_object ());
            }
        }
    }

    if (schema.has ("dependencies"))
        not_implemented ();

    // properties must be checked after the 'properties' check has a chance to
    // create the defaulted entries.
    auto maxProperties = schema.find ("maxProperties");
    if (maxProperties != schema.cend ())
        if (node.size () > maxProperties->second->as_uint ())
            throw json::schema_error ("maxProperties");

    auto minProperties = schema.find ("minProperties");
    if (minProperties != schema.cend ())
        if (node.size () < minProperties->second->as_uint ())
            throw json::schema_error ("minProperties");

    auto required = schema.find ("required");
    if (required != schema.cend ())
        for (const auto &i: required->second->as_array ())
            if (!node.has (i.as_string ()))
                throw json::schema_error ("required");
}


//-----------------------------------------------------------------------------
static void
validate (json::tree::array &node,
          const json::tree::object &schema)
{
    // attempt to match the item and additionalItem schemas
    auto items = schema.find ("items");
    auto additional = schema.find ("additionalItems");

    if (items != schema.cend ()) {
        // items is an object, test all elements with it as a schema
        if (items->second->is_object ()) {
            for (auto &i: node)
                validate (i, items->second->as_object ());
        // items is a list of schemas, test n-elements with it as a schema
        } else if (items->second->is_array ()) {
            const auto &itemArray = items->second->as_array ();

            size_t i = 0;
            for (; i < itemArray.size () && i < node.size (); ++i)
                validate (node[i], itemArray[i].as_object ());

            // we've exhausted the schema list, use the additional schema
            if (i == itemArray.size ()) {
                if (additional->second->is_boolean ()) {
                    if (!additional->second->as_boolean ())
                        throw json::schema_error ("additional");
                } else if (additional->second->is_object ()) {
                    for ( ; i < node.size (); ++i)
                        validate (node[i], additional->second->as_object ());
                } else {
                    throw json::schema_error ("items");
                }
            }
        }
    }

    auto maxItems = schema.find ("maxItems");
    if (maxItems != schema.cend ())
        if (node.size () > maxItems->second->as_uint ())
            throw json::schema_error ("maxItems");

    auto minItems = schema.find ("minItems");
    if (minItems != schema.cend ())
        if (node.size () < minItems->second->as_uint ())
            throw json::schema_error ("minItems");

    // check all element are unique
    // XXX: uses a naive n^2 brute force search on equality because it's 2am
    // and I don't want to write a type aware comparator for the sort.
    auto unique = schema.find ("uniqueItems");
    if (unique != schema.cend () && unique->second->as_boolean ())
        for (size_t a = 0; a < node.size (); ++a)
            for (size_t b = a + 1; b < node.size (); ++b)
                if (node[a] == node[b])
                    throw json::schema_error ("uniqueItems");
}


//-----------------------------------------------------------------------------
static void
validate (json::tree::string &node,
          const json::tree::object &schema)
{
    const auto &val = node.native ();

    // check length is less than a maximum
    auto maxLength = schema.find ("maxLength");
    if (maxLength != schema.cend ()) {
        auto cmp = maxLength->second->as_number ().native ();
        if (!is_integer (cmp))
            throw length_error ("maxLength");

        if (val.size () > cmp)
            throw length_error ("maxLength");
    }

    // check length is greater than a maximum
    auto minLength = schema.find ("minLength");
    if (minLength != schema.cend ()) {
        auto cmp = minLength->second->as_number ().native ();
        if (!is_integer (cmp))
            throw length_error ("minLength");

        if (val.size () < cmp)
            throw length_error ("minLength");
    }

    // check the string conforms to a regex
    // Note: this uses the c++11 regex engine which slightly differs from ECMA 262
    auto pattern = schema.find ("pattern");
    if (pattern != schema.cend ()) {
        std::regex r (pattern->second->as_string ().native (),
                      std::regex_constants::ECMAScript);
        if (!std::regex_search (val, r))
            throw format_error ("pattern");
    }
}


//-----------------------------------------------------------------------------
static void
validate (json::tree::number &node,
          const json::tree::object &schema)
{
    const auto &val = node.native ();

    // check strictly positive integer multiple
    auto mult = schema.find ("multipleOf");
    if (mult != schema.cend ()) {
        auto div = mult->second->as_number ().native ();

        if (val <= 0 || almost_equal (val, div))
            throw json::schema_error ("multipleOf");
    }

    // check maximum holds. exclusive requires max condition.
    auto max = schema.find ("maximum");
    auto exclusiveMax = schema.find ("exclusiveMaximum");
    if (max != schema.end ()) {
        auto cmp = max->second->as_number ().native ();

        if (exclusiveMax != schema.end () && exclusiveMax->second->as_boolean () && val >= cmp)
            throw json::schema_error ("exclusiveMax");
        else if (val > cmp)
            throw json::schema_error ("maximum");
    } else {
        if (exclusiveMax != schema.cend ())
            throw json::schema_error ("exclusiveMax");
    }

    // check minimum holds. exclusive requires min condition
    auto min = schema.find ("minimum");
    auto exclusiveMin = schema.find ("exclusiveMinimum");
    if (min != schema.end ()) {
        auto cmp = min->second->as_number ().native ();

        if (exclusiveMin != schema.end () && exclusiveMin->second->as_boolean () && val <= cmp)
            throw json::schema_error ("exclusiveMin");
        else if (val < cmp)
            throw json::schema_error ("minimum");
    } else {
        if (exclusiveMin != schema.cend ())
            throw json::schema_error ("exclusiveMin");
    }
}


//-----------------------------------------------------------------------------
static void
validate (json::tree::boolean&,
          const json::tree::object&)
{ ; }


//-----------------------------------------------------------------------------
static void
validate (json::tree::null&,
          const json::tree::object&)
{ ; }


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
static void
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
            throw json::schema_error ("enum");
    }

    // check the value is the correct type
    auto type = schema.find ("type");
    if (type != schema.cend ()) {
        // check against a single named type
        if (type->second->is_string ()) {
            auto a = type->second->as_string ();
            auto b = to_string (node.type ());

            if (a != b)
                throw json::schema_error ("type");
        // check against an array of types
        } else if (type->second->is_array ()) {
            auto pos = std::find_if (type->second->as_array ().begin (),
                                     type->second->as_array ().end (),
                                     [&] (const auto &i) { return i.as_string () == to_string (node.type ()); });
            if (pos == type->second->as_array ().end ())
                throw json::schema_error ("type");
        } else
            throw json::schema_error ("type");
    }

    auto allOf = schema.find ("allOf");
    if (allOf != schema.cend ()) {
        for (const auto &i: allOf->second->as_array ())
            validate (node, i.as_object ());
    }

    auto anyOf = schema.find ("anyOf");
    if (anyOf != schema.cend ()) {
        bool success = false;
        for (const auto &i: anyOf->second->as_array ()) {
            try {
                validate (node, i.as_object ());
                success = true;
                break;
            } catch (const json::schema_error&)
            { continue; }
        }

        if (!success)
            throw json::schema_error ("anyOf");
    }

    auto oneOf = schema.find ("oneOf");
    if (oneOf != schema.cend ()) {
        unsigned count = 0;

        for (const auto &i: oneOf->second->as_array ()) {
            try {
                validate (node, i.as_object ());
                count++;
            } catch (const json::schema_error&)
            { ; }

            if (count > 1)
                throw json::schema_error ("oneOf");
        }

        if (count != 1)
            throw json::schema_error ("oneOf");
    }

    auto notSchema = schema.find ("not");
    if (notSchema != schema.cend ()) {
        for (const auto &i: notSchema->second->as_array ()) {
            bool valid = false;
            try {
                validate (node, i.as_object ());
                valid = true;
            } catch (const json::schema_error&)
            { ; }

            if (valid)
                throw json::schema_error ("not");
        }
    }

    switch (node.type ()) {
        case json::tree::OBJECT:    validate (node.as_object (),     schema); return;
        case json::tree::ARRAY:     validate (node.as_array (),      schema); return;
        case json::tree::STRING:    validate (node.as_string (),     schema); return;
        case json::tree::NUMBER:    validate (node.as_number (),     schema); return;
        case json::tree::BOOLEAN:   validate (node.as_boolean (),    schema); return;
        case json::tree::NONE:      validate (node.as_null (),       schema); return;
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
void
json::schema::validate (json::tree::node &data,
                        const json::tree::object &schema)
{
    auto title = schema.find ("title");
    if (title != schema.cend ())
        if (!title->second->is_string ())
            throw json::schema_error ("title");

    auto description = schema.find ("description");
    if (description != schema.cend ())
        if (!description->second->is_string ())
            throw json::schema_error ("description");

    return ::validate (data, schema.as_object ());
}
