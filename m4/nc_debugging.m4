AC_DEFUN([NC_DEBUGGING],[
    ##-------------------------------------------------------------------------
    AX_REQUIRE_DEFINED([AS_HELP_STRING])
    AX_REQUIRE_DEFINED([AX_APPEND_COMPILE_FLAGS])
    AX_REQUIRE_DEFINED([AX_APPEND_LINK_FLAGS])
    AX_REQUIRE_DEFINED([NC_APPEND_ONE_COMPILE_FLAG])

    ##-------------------------------------------------------------------------
    AC_ARG_ENABLE([sanitizer], [
        AS_HELP_STRING([--enable-sanitizer], [enable memory sanitizer])
    ])

    AS_IF([test "x$enable_sanitizer" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fsanitize=address], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fsanitize=undefined], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fsanitize=leak], [], [-Werror])

        AX_APPEND_LINK_FLAGS([-fsanitize=address], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fsanitize=undefined], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fsanitize=leak], [], [-Werror])

        AX_APPEND_LINK_FLAGS([-lasan], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-lubsan], [], [-Werror])
    ])

    ##-------------------------------------------------------------------------
    AC_ARG_ENABLE([debugging], [
        AS_HELP_STRING([--enable-debugging], [enables developer debugging support])
    ])

    AS_IF([test "x${enable_debugging}" = "xyes"], [
        AC_DEFINE([ENABLE_DEBUGGING], [], [Debugging support enabled])
        AC_DEFINE([_GLIBCXX_DEBUG],   [], [Use glibcxx debugging mode])

        NC_APPEND_ONE_COMPILE_FLAG([-O0])
    ], [
        AX_APPEND_COMPILE_FLAGS([-O2])
    ])

    ##-------------------------------------------------------------------------
    AX_APPEND_COMPILE_FLAGS([-g], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-ggdb], [], [-Werror])
])
