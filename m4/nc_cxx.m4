AC_DEFUN([NC_CXX],[
    AC_REQUIRE([AX_COMPILER_VENDOR])

    AX_CHECK_COMPILE_FLAG(
        [-std=c++1z],
        [AX_APPEND_FLAG([-std=c++1z], [CXXFLAGS])],
        [AC_MSG_ERROR([Unable to enable c++1z])]
    )

    AX_APPEND_COMPILE_FLAGS([-pipe], [], [-Werror])

    AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list], [], [-Werror])

    AC_C_CONST
    AC_C_RESTRICT
    AC_C_INLINE

    AX_COMPILER_VENDOR
    AS_IF(
        [test "x$ax_cv_cxx_compiler_vendor" == "xclang"],
        [AX_APPEND_COMPILE_FLAGS([-stdlib=libc++])]
    )

    AX_APPEND_COMPILE_FLAGS([-fvisibility=hidden], [], [-Werror])
])
