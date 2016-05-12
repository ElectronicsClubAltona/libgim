AC_DEFUN([NC_DEBUGGING],[
    ##-------------------------------------------------------------------------
    AX_REQUIRE_DEFINED([AS_HELP_STRING])
    AX_REQUIRE_DEFINED([AX_APPEND_COMPILE_FLAGS])
    AX_REQUIRE_DEFINED([AX_APPEND_LINK_FLAGS])

    ##-------------------------------------------------------------------------
    AC_ARG_ENABLE([sanitizer], [AS_HELP_STRING([--enable-sanitizer], [enable memory sanitizer])])

    AS_IF([test "x$enable_sanitizer" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fsanitize=address])
        AX_APPEND_COMPILE_FLAGS([-fsanitize=undefined])

        AX_APPEND_COMPILE_FLAGS([-ftrapv])
    ])

    AM_CONDITIONAL([WITH_SANITIZER], [test "x${enable_sanitizer}" == "xyes"])

    ##-------------------------------------------------------------------------
    AC_ARG_ENABLE([debugging], [AS_HELP_STRING([--enable-debugging], [enables developer debugging support])])

    AS_IF([test "x${enable_debugging}" = "xyes"], [
        AC_DEFINE([ENABLE_DEBUGGING], [], [Debugging support enabled])
        AC_DEFINE([_GLIBCXX_DEBUG],   [], [Use glibcxx debugging mode])

        AX_APPEND_COMPILE_FLAGS([-O0])

        AX_APPEND_COMPILE_FLAGS([-fstack-protector])
    ], [
        AX_APPEND_COMPILE_FLAGS([-O2])
        AX_APPEND_COMPILE_FLAGS([-fno-rtti])
        AC_DEFINE([NO_RTTI], [], [RTTI disabled])
    ])

    ##-------------------------------------------------------------------------
    AS_CASE([${host_os}],
        [mingw*], [AX_APPEND_COMPILE_FLAGS([-gstabs])],
        [
            AX_APPEND_COMPILE_FLAGS([-g])
            AX_APPEND_COMPILE_FLAGS([-ggdb])
        ]
    )
])
