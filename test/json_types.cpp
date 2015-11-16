#include "json/tree.hpp"

#include "debug.hpp"
#include "maths.hpp"
#include "tap.hpp"

#include <memory>
#include <cstdlib>

int
main (void) {
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
    CHECK (ptr->is_object ());

    const json::tree::node &ref = *ptr;

    CHECK ( ref["string"].is_string ());
    CHECK (!ref["string"].is_array ());
    CHECK (!ref["string"].is_boolean ());
    CHECK (!ref["string"].is_null ());
    CHECK (!ref["string"].is_number ());
    CHECK (!ref["string"].is_object ());
    CHECK_EQ   ( ref["string"].as_string (), "brad");

    CHECK ( ref["integer"].is_number ());
    CHECK (!ref["integer"].is_array ());
    CHECK (!ref["integer"].is_boolean ());
    CHECK (!ref["integer"].is_null ());
    CHECK (!ref["integer"].is_object ());
    CHECK (!ref["integer"].is_string ());
    CHECK (
        util::exactly_equal (
            (unsigned)ref["integer"].as_number ().native (),
            1u
        )
    );

    CHECK ( ref["null"].is_null ());
    CHECK (!ref["null"].is_array ());
    CHECK (!ref["null"].is_boolean ());
    CHECK (!ref["null"].is_number ());
    CHECK (!ref["null"].is_object ());
    CHECK (!ref["null"].is_string ());

    CHECK ( ref["false"].is_boolean ());
    CHECK (!ref["false"].is_array ());
    CHECK (!ref["false"].is_null ());
    CHECK (!ref["false"].is_number ());
    CHECK (!ref["false"].is_object ());
    CHECK (!ref["false"].is_string ());
    CHECK_EQ   ( ref["false"].as_boolean (), false);

    CHECK ( ref["true"].is_boolean ());
    CHECK (!ref["true"].is_array ());
    CHECK (!ref["true"].is_null ());
    CHECK (!ref["true"].is_number ());
    CHECK (!ref["true"].is_object ());
    CHECK (!ref["true"].is_string ());
    CHECK_EQ   ( ref["true"].as_boolean (), true);

    CHECK ( ref["double"].is_number ());
    CHECK (!ref["double"].is_array ());
    CHECK (!ref["double"].is_boolean ());
    CHECK (!ref["double"].is_null ());
    CHECK (!ref["double"].is_object ());
    CHECK (!ref["double"].is_string ());
    CHECK (
        util::exactly_equal (
            ref["double"].as_number ().native (),
            3.14
        )
    );

    CHECK ( ref["object"].is_object ());
    CHECK (!ref["object"].is_array ());
    CHECK (!ref["object"].is_boolean ());
    CHECK (!ref["object"].is_null ());
    CHECK (!ref["object"].is_number ());
    CHECK (!ref["object"].is_string ());

    CHECK ( ref["array"].is_array ());
    CHECK (!ref["array"].is_boolean ());
    CHECK (!ref["array"].is_null ());
    CHECK (!ref["array"].is_number ());
    CHECK (!ref["array"].is_object ());
    CHECK (!ref["array"].is_string ());

    util::TAP::logger tap;
    tap.skip ("convert to TAP");
}
