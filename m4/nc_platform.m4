AC_DEFUN([NC_PLATFORM],[
    AC_CANONICAL_HOST

    AM_CONDITIONAL([PLATFORM_WIN32], [test "x$host_os" = "xmingw32"])
    AM_CONDITIONAL([PLATFORM_LINUX], [test "x$host_os" = "xlinux-gnu"])

    AS_CASE([$host_os],
        [mingw32], [
            AC_DEFINE([WIN32_LEAN_AND_MEAN], [], [reduce win32 header complexity])

            AC_CHECK_HEADERS([winsock2.h ws2tcpip.h])
            AC_CHECK_HEADERS([windows.h],[],[],[AC_INCLUDES_DEFAULT])
            AC_SUBST([NET_LIBS], [-lws2_32])

            AX_APPEND_COMPILE_FLAGS([-DBOOST_THREAD_USE_LIB], [], [-Werror])

            ## Force as many static libraries as possible to avoid bullshit libc
            ## cross-library memory management issues.
            AX_APPEND_LINK_FLAGS([-static-libstdc++], [], [-Werror])
            AX_APPEND_LINK_FLAGS([-static-libgcc], [], [-Werror])
        ],

        [linux-gnu], [ ],

        [AC_MSG_ERROR([Unknown platform])]
    )

    AX_APPEND_COMPILE_FLAGS([-fno-common], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-fno-nonansi-builtins], [], [-Werror])
])
