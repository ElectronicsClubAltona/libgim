AC_DEFUN([NC_OPTIMISATION],[
    ##-------------------------------------------------------------------------
    AX_REQUIRE_DEFINED([AX_APPEND_COMPILE_FLAGS])
    AX_REQUIRE_DEFINED([AX_COMPILER_VENDOR])

    AX_COMPILER_VENDOR

    ##-------------------------------------------------------------------------
    ## Enable LTO

    AC_ARG_ENABLE([lto], [AS_HELP_STRING([--enable-lto], [enable link-time optimisation])])

    AS_IF([test "x$enable_lto" == "xyes"], [
        AS_IF([test "x$ax_cv_cxx_compiler_vendor" == "xgnu"], [
            AC_CHECK_TOOLS([RANLIB], [gcc-ranlib ranlib])
            AC_CHECK_TOOLS([AR], [gcc-ar ar])
            AC_CHECK_TOOLS([NM], [gcc-nm nm])
        ])

        AX_APPEND_COMPILE_FLAGS([-flto], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fno-fat-lto-objects], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-flto-odr-type-merging], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fuse-linker-plugin], [], [-Werror])

        AX_APPEND_COMPILE_FLAGS([-fdevirtualize-at-ltrans], [], [-Werror])
    ])

    ##-------------------------------------------------------------------------
    ## Enable frame pointer

    AC_ARG_ENABLE([frame-pointer], [AS_HELP_STRING([--enable-frame-pointer], [retain the frame pointer even if optimising])])

    AS_IF([test "x$enable_frame_pointer" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fno-omit-frame-pointer], [], [-Werror])
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

    # base instruction set requirements for x86
    AX_APPEND_COMPILE_FLAGS([-mfpmath=sse], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-msahf], [], [-Werror])

    ##-------------------------------------------------------------------------
    ## Enable aggressive code generation optimisations

    AS_IF([test "x$enable_debugging" != "xyes"], [
        # gcc vectorisation
        AX_APPEND_COMPILE_FLAGS([-ftree-vectorize], [], [-Werror])

        # clang vectorisation
        AX_APPEND_COMPILE_FLAGS([-fvectorize], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fslp-vectorize], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fslp-vectorize-aggressive], [], [-Werror])

        # loop hosting/distribution
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-distribute-patterns], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-distribution], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-if-convert-stores], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ftree-loop-linear], [], [-Werror])

        AX_APPEND_COMPILE_FLAGS([-funsafe-loop-optimizations], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-floop-interchange], [], [-Werror])

        # safety removal for performance
        AX_APPEND_COMPILE_FLAGS([-fno-stack-protector], [], [-Werror])
    ])

    ##-------------------------------------------------------------------------
    ## Enable code size optimisations (that don't impact performance)
    ## Note: we assume CXX, and that CXXLINK is g++ not ld, hence the -Wl opt
    AS_IF([test "x$enable_debugging" != "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fdevirtualize], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fdevirtualize-speculatively], [], [-Werror])

        AX_CHECK_LINK_FLAG([-Wl,--gc-sections], [
            AX_APPEND_COMPILE_FLAGS([-fdata-sections], [], [-Werror])
            AX_APPEND_COMPILE_FLAGS([-ffunction-sections], [], [-Werror])
            AX_APPEND_LINK_FLAGS([-Wl,--gc-sections], [], [-Werror])
        ])
    ])
])
