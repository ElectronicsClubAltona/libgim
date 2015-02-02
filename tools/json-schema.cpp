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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */


#include "../json.hpp"

#include "../debug.hpp"
#include "../maths.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <regex>

#include <boost/filesystem.hpp>

using namespace std;
using namespace std::placeholders;

enum {
    ARG_COMMAND,
    ARG_SCHEMA,
    ARG_INPUT,

    NUM_ARGS
};


void
print_usage (int argc, char **argv) {
    if (argc <= ARG_COMMAND)
        abort ();

    std::cerr << "Usage: " << argv[ARG_COMMAND] << " <schema> <input>\n";
}


const char*
type_to_string (const json::tree::node &node) {
    if (node.is_array   ()) return "array";
    if (node.is_boolean ()) return "boolean";
    if (node.is_null    ()) return "null";
    if (node.is_number  ()) return "number";
    if (node.is_object  ()) return "object";
    if (node.is_string  ()) return "string";

    unreachable ();
}


bool
is_node_valid (const json::tree::node   &node,
               const json::tree::object &schema);


bool
is_type_valid (const json::tree::node &node,
               const json::tree::node &type) {
    if (type.is_array ()) {
        return any_of (type.as_array ().begin (),
                       type.as_array ().end (),
                       bind (is_type_valid, ref (node), _1));
    }

    if (!type.is_string ())
        throw json::tree::schema_error ("schema type requires array, string, or object");

    static const auto ANY_VALIDATOR = [] (const json::tree::node &) { return true; };
    static const auto INT_VALIDATOR = [] (const json::tree::node &n) {
        return n.is_number () && is_integer (n.as_number ());
    };

    static const map<string, function<bool(const json::tree::node&)>> TYPE_VALIDATORS ({
        { "array",   bind (&json::tree::node::is_array,   _1) },
        { "boolean", bind (&json::tree::node::is_boolean, _1) },
        { "null",    bind (&json::tree::node::is_null,    _1) },
        { "number",  bind (&json::tree::node::is_number,  _1) },
        { "object",  bind (&json::tree::node::is_object,  _1) },
        { "string",  bind (&json::tree::node::is_string,  _1) },
        { "any",     ANY_VALIDATOR },
        { "integer", INT_VALIDATOR },
    });

    auto pos = TYPE_VALIDATORS.find (type.as_string ().native ());
    if (pos == TYPE_VALIDATORS.end ()) {
        std::cerr << "warning: unknown type " << type.as_string ().native () << ". assuming valid.\n";
        return true;
    }

    return pos->second(node);
}


bool
is_disallow_valid (const json::tree::node &node,
                   const json::tree::node &constraint)
    { return !is_type_valid (node, constraint); }


bool
is_enum_valid (const json::tree::node &node,
               const json::tree::node &constraint) {
    if (!constraint.is_array ())
        throw json::tree::schema_error ("enum validation requires an array");

    const json::tree::array &valids = constraint.as_array ();
    return valids.end () != std::find (valids.begin (),
                                       valids.end   (),
                                       node);
}


bool
is_enum_valid (const json::tree::string &node,
               const json::tree::node   &constraint) {
    return is_enum_valid (static_cast<const json::tree::node&> (node), constraint);
}


bool
is_always_valid (const json::tree::node &,
                 const json::tree::node &)
    { return true; }


/*static const map<string, validator_t> COMMON_VALIDATORS ({
    { "description", &is_always_valid   },
    { "disallow",    &is_disallow_valid },
    { "enum",        &is_enum_valid     },
    { "title",       &is_always_valid   },
    { "type",        &is_type_valid     },
});*/



bool
is_boolean_valid (const json::tree::node   &node,
                  const json::tree::object &)
    { return node.is_boolean (); }


bool
is_null_valid (const json::tree::node   &node,
               const json::tree::object &)
    { return node.is_null (); }


//
// JSON number
//

bool
is_minimum_valid (const json::tree::number &node,
                  const json::tree::node   &constraint) {
    return constraint["minimum"].as_number () <= node;
}


bool
is_maximum_valid (const json::tree::number &node,
                  const json::tree::node   &constraint) {
    return constraint["maximum"].as_number () >= node;
}


bool
is_exclusive_minimum_valid (const json::tree::number &node,
                            const json::tree::node   &constraint) {
    return constraint["exclusiveMinimum"].as_number () < node;
}


bool
is_exclusive_maximum_valid (const json::tree::number &node,
                            const json::tree::node   &constraint) {
    return constraint["exclusiveMaximum"].as_number () > node;
}


bool
is_divisible_by_valid (const json::tree::number &node,
                       const json::tree::node   &constraint) {
    return exactly_equal (fmod (node.native (),
                                constraint["divisibleBy"].as_number ()),
                          0.0);
}


bool
is_number_valid (const json::tree::number &node,
                 const json::tree::object &schema) {
    typedef bool (*number_validator_t)(const json::tree::number&, const json::tree::node&);
    static const map<string, number_validator_t> VALIDATORS = {
        { "minimum",          &is_minimum_valid }, 
        { "maximum",          &is_maximum_valid }, 
        { "exclusiveMinimum", &is_exclusive_minimum_valid }, 
        { "exclusiveMaximum", &is_exclusive_maximum_valid }, 
        { "divisibleBy",      &is_divisible_by_valid  }, 
    };

    for (const auto &i: schema) {
        const std::string &key       = i.first;
        const auto        &validator = VALIDATORS.find (key);
        if (validator == VALIDATORS.end ()) {
            std::cerr << "Unknown validation constraint: " << key << "\n";
            continue;
        }

        if (!validator->second (node, schema))
            return false;
    }

    return true;
}


//
// JSON string
//


bool
is_min_length_valid (const json::tree::string &node,
                     const json::tree::node   &constraint) {
    if (!is_integer (constraint))
        return false;

    return node.size () >= constraint.as_number ();
}


bool
is_max_length_valid (const json::tree::string &node,
                     const json::tree::node   &constraint) {
    if (!is_integer (constraint))
        return false;

    return node.size () <= constraint.as_number ();
}


bool
is_pattern_valid (const json::tree::string &node,
                  const json::tree::node   &constraint) {
    if (!constraint.is_string ())
        return false;

    regex pattern (constraint.as_string ().native (),
                   regex_constants::ECMAScript);
    return regex_match (node.native (), pattern);
}


bool
is_string_valid (const json::tree::string &node,
                 const json::tree::object &schema) {
    typedef bool (*string_validator_t)(const json::tree::string&, const json::tree::node&);
    static const map<std::string, string_validator_t> VALIDATORS  = {
        { "minLength", &is_min_length_valid },
        { "maxLength", &is_max_length_valid },
        { "pattern",   &is_pattern_valid },
        { "enum",      &is_enum_valid },
    };

    for (const json::tree::object::const_iterator::value_type &i: schema) {
        const std::string &key        =  i.first;
        const json::tree::node  &constraint = *i.second;

        auto validator = VALIDATORS.find (key);
        if (validator == VALIDATORS.end ()) {
            std::cerr << "Unknown validation constraint: " << key << "\n";
            continue;
        }

        if (!validator->second (node, constraint)) {
            std::cerr << "Failed string constraint: " << key << "\n";
            return false;
        }
    }

    return true;
}


bool
is_string_valid (const json::tree::node   &node,
                 const json::tree::object &schema) {
    if (!node.is_string ())
        return false;
    return is_string_valid (node.as_string (), schema);
}

//
// JSON array
//


bool
is_max_items_valid (const json::tree::array &node,
                    const json::tree::node  &constraint) {
    if (!constraint.is_number () && is_integer (constraint.as_number ()))
        throw json::tree::schema_error ("max_items should be an integer");

    return node.size () <= constraint.as_number ();
}


bool
is_min_items_valid (const json::tree::array &node,
                    const json::tree::node  &constraint) {
    if (!constraint.is_number () && is_integer (constraint.as_number ()))
        throw json::tree::schema_error ("min_items should be an integer");

    return node.size () >= constraint.as_number ();
}


bool
is_unique_items_valid (const json::tree::array &node,
                       const json::tree::node  &constraint) {
    if (!constraint.is_boolean ())
        throw json::tree::schema_error ("uniqueItems must be a boolean");

    if (node.size () < 2) 
        return true;


    for (json::tree::array::const_iterator i = node.begin (); i != node.end () - 1; ++i) {
        if (find (i + 1, node.end (), *i) != node.end ())
            return false;
    }

    return true;
}


bool
is_items_valid (const json::tree::array &node,
                const json::tree::node  &_schema) {
    if (!_schema.is_object ())
        throw json::tree::schema_error ("array_items constraint must be an object");
    const json::tree::object &schema = _schema.as_object ();

    for (const json::tree::node &i: node)
        if (!is_node_valid (i, schema))
            return false;

    return true;

    not_implemented ();
    return false;
}


bool
is_additional_items_valid (const json::tree::array &,
                           const json::tree::node  &) {
    not_implemented ();
    return false;
}


bool
is_array_valid (const json::tree::array  &node,
                const json::tree::object &schema) {
    CHECK (node.is_array ());

    typedef bool (*array_validator_t)(const json::tree::array&, const json::tree::node&);
    static const map<string, array_validator_t> VALIDATORS ({
        { "items",           &is_items_valid            },
        { "minItems",        &is_min_items_valid        },
        { "maxItems",        &is_max_items_valid        },
        { "uniqueItems",     &is_unique_items_valid     },
        { "additionalItems", &is_additional_items_valid },
    });

    for (const json::tree::object::const_iterator::value_type &i: schema) {
        const std::string &key        = i.first;
        const json::tree::node  &constraint = *i.second;

        auto validator = VALIDATORS.find (key);
        if (validator == VALIDATORS.end ()) {
            std::cerr << "Ignoring unknown contraint key: " << key << "\n";
            continue;
        }

        if (!validator->second (node, constraint)) {
            std::cerr << "Failed validating array constraint: " << key << "\n";
            return false;
        }
    }

    return true;
}


//
// JSON object
//

bool
is_properties_valid (const json::tree::object &node,
                     const json::tree::object &schema) {
    for (const json::tree::object::const_iterator::value_type &element: node) {
        const std::string &key =  element.first;
        const json::tree::node  &val = *element.second;

        if (!schema.has (key)) {
            std::cerr << "[warning]  no constraint found for key: " << key << "\n";
            continue;
        }

        if (!is_node_valid (val, schema[key].as_object ())) {
            std::cerr << "failed validation on property: " << key << "\n";
            return false;
        }
    }

    return true;
}


bool
is_properties_valid (const json::tree::object &node,
                     const json::tree::node   &constraint) {
    CHECK (node.is_object ());

    if (!constraint.is_object ())
        throw json::tree::schema_error ("properties needs an object");

    return is_properties_valid (node, constraint.as_object ());
}


bool
is_object_valid (const json::tree::object &node,
                 const json::tree::object &schema) {
    typedef bool (*object_validator_t)(const json::tree::object&, const json::tree::node&);
    static const map<string, object_validator_t> VALIDATORS = {
        { "properties",  &is_properties_valid },
        //{ "patternProperties", &is_pattern_properties_valid },
        //{ "additionalProperties", &is_additionaL_properties_valid },
    };

    for (const json::tree::object::const_iterator::value_type &i: schema) {
        const std::string &name       =  i.first;
        const json::tree::node  &constraint = *i.second;

        auto validator = VALIDATORS.find (name);
        if (validator == VALIDATORS.end ()) {
            std::cerr << "Unknown constraint name \"" << name << "\". Ignoring.\n";
            continue;
        }

        if (!validator->second(node, constraint)) {
            std::cerr << "Failed validation on: " << name << "\n";
            return false;
        }
    }

    return true;
}


bool
is_object_valid (const json::tree::node   &node,
                 const json::tree::object &schema) {
    if (!node.is_object ())
        return false;

    return is_object_valid (node.as_object (), schema);
}


//
// JSON node
//

bool
is_node_valid (const json::tree::node   &node,
               const json::tree::object &schema) {
    if (schema.has ("$ref")) {
        const std::string &uri = schema["$ref"].as_string ();
        std::cerr << "loading referenced schema: " << uri << "\n";

        if (uri[0] == '#') {
            std::cerr << "[error] schema fragments are not supported\n";
            return false;
        }

        auto referenced = json::tree::parse (boost::filesystem::path (uri));
        return is_node_valid (node, referenced->as_object ());
    }

    if (schema.has ("type") && 
       !is_type_valid (node, schema["type"]))
    {
        std::cerr << "node type is \"" << type_to_string (node) << "\", expected " << schema["type"] << "\n";
        return false;
    }

#define IS_VALID(T)                                             \
    do {                                                        \
        if (node.is_##T ()) {                                   \
            if (!is_##T##_valid (node.as_##T (), schema)) {     \
                std::cerr << "Failed validation as " #T "\n";   \
                return false;                                   \
            }                                                   \
            return true;                                        \
        }                                                       \
    } while (0)

    IS_VALID(array);
    IS_VALID(boolean);
    IS_VALID(null);
    IS_VALID(number);
    IS_VALID(object);
    IS_VALID(string);

#undef IS_VALID

    return false;
    

    /*static const map<string, validator_t> VALIDATORS ({
        { "description", &is_always_valid   },
        { "disallow",    &is_disallow_valid },
        { "enum",        &is_enum_valid     },
        { "title",       &is_always_valid   },
        { "type",        &is_type_valid     },
    });*/

    

    //"required";

    return false;
}


bool
is_root_valid (const json::tree::node   &node,
               const json::tree::object &schema) {
    if (!node.is_array () && !node.is_object ())
        return false;
    return is_node_valid (node, schema);
}


//
// Driver
//

int
main (int argc, char **argv) {
    // Basic argument checking
    if (argc != NUM_ARGS) {
        print_usage (argc, argv);
        return EXIT_FAILURE;
    }
    
    // Load the schema and input
    unique_ptr<json::tree::node> schema, input;
    try {
        schema = json::tree::parse (boost::filesystem::path (argv[ARG_SCHEMA]));
        input  = json::tree::parse (boost::filesystem::path (argv[ARG_INPUT]));
    } catch (const json::tree::parse_error &err) {
        std::cerr << "malformed json for schema or input. " << err.what () << "\n";
        return EXIT_FAILURE;
    }

    // Check schema is valid
    if (!schema->is_object ()) {
        std::cerr << "Schema should be an object\n";
        return EXIT_FAILURE;
    }
    
    const json::tree::object &schema_object = schema->as_object ();

    // Check input is valid
    if (!is_node_valid (*input, schema_object)) {
        std::cerr << "input does not satisfy the schema\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

