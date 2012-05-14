
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
    CHECK_HARD (ptr->is_object ());

    const json::node &ref = *ptr;

    CHECK_HARD ( ref["string"].is_string ());
    CHECK_HARD (!ref["string"].is_array ());
    CHECK_HARD (!ref["string"].is_boolean ());
    CHECK_HARD (!ref["string"].is_null ());
    CHECK_HARD (!ref["string"].is_number ());
    CHECK_HARD (!ref["string"].is_object ());
    CHECK_EQ   ( ref["string"].as_string (), "brad");

    CHECK_HARD ( ref["integer"].is_number ());
    CHECK_HARD (!ref["integer"].is_array ());
    CHECK_HARD (!ref["integer"].is_boolean ());
    CHECK_HARD (!ref["integer"].is_null ());
    CHECK_HARD (!ref["integer"].is_object ());
    CHECK_HARD (!ref["integer"].is_string ());
    CHECK_EQ   ( ref["integer"].as_number (), 1u);

    CHECK_HARD ( ref["null"].is_null ());
    CHECK_HARD (!ref["null"].is_array ());
    CHECK_HARD (!ref["null"].is_boolean ());
    CHECK_HARD (!ref["null"].is_number ());
    CHECK_HARD (!ref["null"].is_object ());
    CHECK_HARD (!ref["null"].is_string ());

    CHECK_HARD ( ref["false"].is_boolean ());
    CHECK_HARD (!ref["false"].is_array ());
    CHECK_HARD (!ref["false"].is_null ());
    CHECK_HARD (!ref["false"].is_number ());
    CHECK_HARD (!ref["false"].is_object ());
    CHECK_HARD (!ref["false"].is_string ());
    CHECK_EQ   ( ref["false"].as_boolean (), false);

    CHECK_HARD ( ref["true"].is_boolean ());
    CHECK_HARD (!ref["true"].is_array ());
    CHECK_HARD (!ref["true"].is_null ());
    CHECK_HARD (!ref["true"].is_number ());
    CHECK_HARD (!ref["true"].is_object ());
    CHECK_HARD (!ref["true"].is_string ());
    CHECK_EQ   ( ref["true"].as_boolean (), true);

    CHECK_HARD ( ref["double"].is_number ());
    CHECK_HARD (!ref["double"].is_array ());
    CHECK_HARD (!ref["double"].is_boolean ());
    CHECK_HARD (!ref["double"].is_null ());
    CHECK_HARD (!ref["double"].is_object ());
    CHECK_HARD (!ref["double"].is_string ());
    CHECK_EQ   ( ref["double"].as_number (), 3.14);

    CHECK_HARD ( ref["object"].is_object ());
    CHECK_HARD (!ref["object"].is_array ());
    CHECK_HARD (!ref["object"].is_boolean ());
    CHECK_HARD (!ref["object"].is_null ());
    CHECK_HARD (!ref["object"].is_number ());
    CHECK_HARD (!ref["object"].is_string ());

    CHECK_HARD ( ref["array"].is_array ());
    CHECK_HARD (!ref["array"].is_boolean ());
    CHECK_HARD (!ref["array"].is_null ());
    CHECK_HARD (!ref["array"].is_number ());
    CHECK_HARD (!ref["array"].is_object ());
    CHECK_HARD (!ref["array"].is_string ());

    return EXIT_SUCCESS;
}
