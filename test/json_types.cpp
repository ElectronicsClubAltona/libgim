#include "json/tree.hpp"

#include "debug.hpp"
#include "maths.hpp"
#include "tap.hpp"

#include <memory>
#include <cstdlib>

int
main (void)
{
    util::TAP::logger tap;

    static const std::string TEST_STRING = R"_(
        {
           "string":"brad",
           "integer":1,
           "null":null,
           "false":false,
           "true":true,
           "double":3.14,
           "object":{
               "test": "test"
           },
           "array":[
               1, 2, 3, 4
           ]
        })_";

    std::unique_ptr<json::tree::node> ptr = json::tree::parse (TEST_STRING);
    tap.expect (ptr->is_object (), "is_object");
    CHECK (ptr->is_object ());

    const json::tree::node &ref = *ptr;

    {
        tap.expect ( ref["string"].is_string (),    "string is_string");
        tap.expect (!ref["string"].is_array (),     "string not is_array");
        tap.expect (!ref["string"].is_boolean (),   "string not is_boolean");
        tap.expect (!ref["string"].is_null (),      "string not is_null");
        tap.expect (!ref["string"].is_number (),    "string not is_number");
        tap.expect (!ref["string"].is_object (),    "string not is_object");

        tap.expect_eq (ref["string"].as_string (), "brad", "string value equality");
    }

    {
        tap.expect ( ref["integer"].is_number (),   "integer is_number");
        tap.expect (!ref["integer"].is_array (),    "integer not is_array");
        tap.expect (!ref["integer"].is_boolean (),  "integer not is_boolean");
        tap.expect (!ref["integer"].is_null (),     "integer not is_null");
        tap.expect (!ref["integer"].is_object (),   "integer not is_object");
        tap.expect (!ref["integer"].is_string (),   "integer not is_string");
        tap.expect (
            util::exactly_equal (
                (unsigned)ref["integer"].as_number ().as_uint (),
                1u
            ),
            "integer value equality"
        );
    }

    {
        tap.expect ( ref["null"].is_null (),    "null is_null");
        tap.expect (!ref["null"].is_array (),   "null not is_array");
        tap.expect (!ref["null"].is_boolean (), "null not is_boolean");
        tap.expect (!ref["null"].is_number (),  "null not is_number");
        tap.expect (!ref["null"].is_object (),  "null not is_object");
        tap.expect (!ref["null"].is_string (),  "null not is_string");
    }

    {
        tap.expect ( ref["false"].is_boolean (),    "false is_boolean");
        tap.expect (!ref["false"].is_array (),      "false not is_array");
        tap.expect (!ref["false"].is_null (),       "false not is_null");
        tap.expect (!ref["false"].is_number (),     "false not is_number");
        tap.expect (!ref["false"].is_object (),     "false not is_object");
        tap.expect (!ref["false"].is_string (),     "false not is_string");

        tap.expect_eq (ref["false"].as_boolean (), false, "false value equality");
    }

    {
        tap.expect ( ref["true"].is_boolean (), "true, is_boolean");
        tap.expect (!ref["true"].is_array (),   "true not is_array");
        tap.expect (!ref["true"].is_null (),    "true not is_null");
        tap.expect (!ref["true"].is_number (),  "true not is_number");
        tap.expect (!ref["true"].is_object (),  "true not is_object");
        tap.expect (!ref["true"].is_string (),  "true not is_string");

        tap.expect_eq ( ref["true"].as_boolean (), true, "true value equality");
    }

    {
        tap.expect ( ref["double"].is_number (), "double is_number");

        tap.expect (!ref["double"].is_array (),   "double not is_array");
        tap.expect (!ref["double"].is_boolean (), "double not is_boolean");
        tap.expect (!ref["double"].is_null (),    "double not is_null");
        tap.expect (!ref["double"].is_object (),  "double not is_object");
        tap.expect (!ref["double"].is_string (),  "double not is_string");
        tap.expect (
            util::exactly_equal (
                ref["double"].as_number ().as<double> (),
                3.14
            ),
            "double value equality"
        );
    }

    {
        tap.expect ( ref["object"].is_object (),    "object is_object");
        tap.expect (!ref["object"].is_array (),     "object not is_array");
        tap.expect (!ref["object"].is_boolean (),   "object not is_boolean");
        tap.expect (!ref["object"].is_null (),      "object not is_null");
        tap.expect (!ref["object"].is_number (),    "object not is_number");
        tap.expect (!ref["object"].is_string (),    "object not is_string");
    }

    {
        tap.expect ( ref["array"].is_array (),      "array is_array");
        tap.expect (!ref["array"].is_boolean (),    "array not is_boolean");
        tap.expect (!ref["array"].is_null (),       "array not is_null");
        tap.expect (!ref["array"].is_number (),     "array not is_number");
        tap.expect (!ref["array"].is_object (),     "array not is_object");
        tap.expect (!ref["array"].is_string (),     "array not is_string");
    }

    return tap.status ();
}
