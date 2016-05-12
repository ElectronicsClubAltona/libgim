AC_DEFUN([NC_WARNINGS],[
    AC_REQUIRE([AX_APPEND_COMPILE_FLAGS])
    AC_REQUIRE([AX_COMPILER_VENDOR])

    AX_COMPILER_VENDOR

    # -------------------------------------------------------------------------
    # Compound warnings
    AX_APPEND_COMPILE_FLAGS([-Wall], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wextra], [], [-Werror])

    # -------------------------------------------------------------------------
    # General warnings
    AX_APPEND_COMPILE_FLAGS([-Wbool-compare], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wcast-align], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wcast-qual], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wdisabled-optimization], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wfloat-conversion], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wfloat-equal], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wno-aggressive-loop-optimizations], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wnoexcept], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wnon-virtual-dtor], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wno-parentheses], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wpointer-arith], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wredundant-decls], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wshadow], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wsign-compare], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wsign-promo], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wstrict-aliasing], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wstrict-overflow], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wtype-limits], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wunsafe-loop-optimizations], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wunused-but-set-variable], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wunused-parameter], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wpessimizing-move], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wswitch-enum], [], [-Werror])

    dnl gcc-6.1.0 warnings
    AX_APPEND_COMPILE_FLAGS([-Wshift-negative-value], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wnull-dereference], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wduplicated-cond], [], [-Werror])

    dnl useless-cast isn't usable on gcc-6.1 due to spurious warnings
    dnl see gcc#70844
    AX_APPEND_COMPILE_FLAGS([-Wuseless-cast], [], [-Werror])

    dnl clang 3.7.1 warnings
    AX_APPEND_COMPILE_FLAGS([-Wshorten-64-to-32], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wdeprecated], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wcovered-switch-default], [], [-Werror])

    # -------------------------------------------------------------------------
    # Required extensions
    #AX_APPEND_COMPILE_FLAGS([-Wgnu-flexible-array-union-member], [], [-Werror])
    #AX_APPEND_COMPILE_FLAGS([-Wno-c99-extensions], [], [-Werror])
    #AX_APPEND_COMPILE_FLAGS([-Wno-vla-extension], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wno-vla], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-Wno-gnu-anonymous-struct])

    AS_IF([test "x$ax_cv_cxx_compiler_vendor" != "xgnu"], [
        AX_APPEND_COMPILE_FLAGS([-Wno-c99-extensions], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-flexible-array-extensions], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-gnu-conditional-omitted-operand], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-gnu-empty-struct], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-gnu-flexible-array-union-member], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-gnu-zero-variadic-macro-arguments], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-vla-extension], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-zero-length-array], [], [-Werror])
    ])

    # -------------------------------------------------------------------------
    # Excessive warnings
    AX_APPEND_COMPILE_FLAGS([-Wno-missing-braces], [], [-Werror])

    AS_IF([test "x$ax_cv_cxx_compiler_vendor" != "xgnu"], [
        AX_APPEND_COMPILE_FLAGS([-Wno-nested-anon-types], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wno-unused-const-variable], [], [-Werror])
    ])

    # -------------------------------------------------------------------------
    # Strict warnings

    # -------------------------------------------------------------------------
    # Advisory warnings
    AC_ARG_ENABLE([advisory], [
        AS_HELP_STRING([--enable-advisory], [enable advisory warnings])
    ])

    AS_IF([test "x$enable_advisory" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-Winline], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wsuggest-final-types], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wsuggest-final-methods], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wsuggest-override], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wweak-vtables], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-Wpadded], [], [-Werror])
    ], [])
])
