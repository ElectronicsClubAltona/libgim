AC_DEFUN([NC_CXX],[
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

    AC_ARG_ENABLE([libcxx], [AS_HELP_STRING([--enable-libcxx], [use clang's cxx library])])

    AS_IF([test "x${enable_libcxx}" == "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-stdlib=libc++])
    ])

    AX_APPEND_COMPILE_FLAGS([-fvisibility=hidden], [], [-Werror])
])
