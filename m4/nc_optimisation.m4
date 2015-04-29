AC_DEFUN([NC_OPTIMISATION],[
    ##-------------------------------------------------------------------------
    AX_REQUIRE_DEFINED([AX_APPEND_COMPILE_FLAGS])
    AX_REQUIRE_DEFINED([AC_CANONICAL_HOST])
    AX_REQUIRE_DEFINED([AX_COMPILER_VENDOR])

    AX_COMPILER_VENDOR

    ##-------------------------------------------------------------------------
    ## Enable LTO

    AC_ARG_ENABLE([lto], [
        AS_HELP_STRING([--enable-lto], [enable link-time optimisation])
    ])

    AS_IF([test "x$enable_lto" == "xyes"], [
        AS_IF([test x"${host_os}" == x"mingw32"], [
            AC_ERROR([mingw32 link-time optimisation is currently broken])
        ])

        AS_IF([test "x$ax_cv_cxx_compiler_vendor" == "xgnu"], [
            AC_CHECK_TOOLS([RANLIB], [gcc-ranlib ranlib])
            AC_CHECK_TOOLS([AR], [gcc-ar ar])
            AC_CHECK_TOOLS([NM], [gcc-nm nm])
        ])

        AX_APPEND_COMPILE_FLAGS([-flto], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fno-fat-lto-objects], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-flto-odr-type-merging], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fuse-linker-plugin], [], [-Werror])
    ])

    ##-------------------------------------------------------------------------
    ## Choose the most performant processor architecture and features

    AC_CANONICAL_HOST

    AS_CASE([$host_cpu],
        [x86_64], [
            AX_APPEND_COMPILE_FLAGS([-mtune=generic], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-msse], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-msse2], [], [-Werror])
        ],

        [i686], [
            AX_APPEND_COMPILE_FLAGS([-march=prescott], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-mtune=generic], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-mcmov], [], [-Werror])
        ],

        [AC_MSG_WARN([unknown host_cpu])]
    )

    AX_APPEND_COMPILE_FLAGS([-pipe])
    AX_APPEND_COMPILE_FLAGS([-mfpmath=sse], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-msahf], [], [-Werror])

    ##-------------------------------------------------------------------------
    ## Enable aggressive code generation optimisations

    AS_IF([test "x$enable_debugging" != "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-distribute-patterns], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-if-convert-stores], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ftree-vectorize], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-funsafe-loop-optimizations], [], [-Werror])

        # gcc >= 4.8 defaults to enabling stack-protector, we care more about
        # performance than security.
        AX_APPEND_COMPILE_FLAGS([-fno-stack-protector], [], [-Werror])
    ])

    ##-------------------------------------------------------------------------
    ## Enable code size optimisations (that don't impact performance)
    ## Note: we assume CXX, and that CXXLINK is g++ not ld, hence the -Wl opt
    AS_IF([test "x$enable_debugging" != "xyes"], [
        AX_CHECK_LINK_FLAG([-Wl,--gc-sections], [
            AX_APPEND_COMPILE_FLAGS([-fdata-sections], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-ffunction-sections], [], [-Werror])
            AX_APPEND_LINK_FLAGS([-Wl,--gc-sections], [], [-Werror])
        ])
    ])
])
