
#include "../debug.hpp"
#include "../json.hpp"

#include <memory>
#include <cstdlib>

int
main (int, char**) {
    static const char TEST_STRING[] =
        "{"
        "   \"string\" :  \"brad\","
        "   \"integer\":  1,       "
        "   \"null\":     null,    "
        "   \"false\":    false,   "
        "   \"true\":     true,    "
        "   \"double\":   3.14,    "
        "   \"object\":   {        "
        "       \"test\": \"test\" "
        "   },                     "
        "   \"array\":     [       "
        "       1, 2, 3, 4         "
        "   ]"
        "}";

    std::unique_ptr<json::node> ptr = json::parse (TEST_STRING);
    check_hard (ptr->is_object ());

    const json::node &ref = *ptr;

    check_hard ( ref["string"].is_string ());
    check_hard (!ref["string"].is_array ());
    check_hard (!ref["string"].is_boolean ());
    check_hard (!ref["string"].is_null ());
    check_hard (!ref["string"].is_number ());
    check_hard (!ref["string"].is_object ());
    check_eq   ( ref["string"].as_string (), "brad");

    check_hard ( ref["integer"].is_number ());
    check_hard (!ref["integer"].is_array ());
    check_hard (!ref["integer"].is_boolean ());
    check_hard (!ref["integer"].is_null ());
    check_hard (!ref["integer"].is_object ());
    check_hard (!ref["integer"].is_string ());
    check_eq   ( ref["integer"].as_number (), 1u);

    check_hard ( ref["null"].is_null ());
    check_hard (!ref["null"].is_array ());
    check_hard (!ref["null"].is_boolean ());
    check_hard (!ref["null"].is_number ());
    check_hard (!ref["null"].is_object ());
    check_hard (!ref["null"].is_string ());

    check_hard ( ref["false"].is_boolean ());
    check_hard (!ref["false"].is_array ());
    check_hard (!ref["false"].is_null ());
    check_hard (!ref["false"].is_number ());
    check_hard (!ref["false"].is_object ());
    check_hard (!ref["false"].is_string ());
    check_eq   ( ref["false"].as_boolean (), false);

    check_hard ( ref["true"].is_boolean ());
    check_hard (!ref["true"].is_array ());
    check_hard (!ref["true"].is_null ());
    check_hard (!ref["true"].is_number ());
    check_hard (!ref["true"].is_object ());
    check_hard (!ref["true"].is_string ());
    check_eq   ( ref["true"].as_boolean (), true);

    check_hard ( ref["double"].is_number ());
    check_hard (!ref["double"].is_array ());
    check_hard (!ref["double"].is_boolean ());
    check_hard (!ref["double"].is_null ());
    check_hard (!ref["double"].is_object ());
    check_hard (!ref["double"].is_string ());
    check_eq   ( ref["double"].as_number (), 3.14);

    check_hard ( ref["object"].is_object ());
    check_hard (!ref["object"].is_array ());
    check_hard (!ref["object"].is_boolean ());
    check_hard (!ref["object"].is_null ());
    check_hard (!ref["object"].is_number ());
    check_hard (!ref["object"].is_string ());

    check_hard ( ref["array"].is_array ());
    check_hard (!ref["array"].is_boolean ());
    check_hard (!ref["array"].is_null ());
    check_hard (!ref["array"].is_number ());
    check_hard (!ref["array"].is_object ());
    check_hard (!ref["array"].is_string ());

    return EXIT_SUCCESS;
}
