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
        m4_foreach([NAME], [
            dnl common top-level names. deliberately does not include
            dnl 'undefined' due to false positives in some core libraries
            dnl (eg, boost::format).
            [address],
            dnl gcc specific names. do not use vptr or boost::format will die
            [alignment],
            [bounds],
            [enum],
            [float-cast-overflow],
            [float-divide-by-zero],
            [integer-divide-by-zero],
            [leak],
            [nonnull-attribute],
            [object-size],
            [return],
            [returns-nonnull-attribute],
            [shift],
            [signed-integer-overflow],
            [unreachable],
            [vla-bound],
            dnl clang specific names
            [integer],
            [thread],
            [undefined-trap],
            [cfi]dnl
        ], [
            AX_APPEND_COMPILE_FLAGS([-fsanitize=[]NAME], [], [-Werror])
            AX_APPEND_LINK_FLAGS([-fsanitize=[]NAME], [], [-Werror])
        ])

        ## We, and the std library, tend to use unsigned overflow legitimately
        AX_APPEND_COMPILE_FLAGS([-fno-sanitize=unsigned-integer-overflow], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fno-sanitize=unsigned-integer-overflow], [], [-Werror])

        AX_APPEND_COMPILE_FLAGS([-ftrapv], [], [-Werror])
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
