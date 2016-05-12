AC_DEFUN([NC_PLATFORM],[
    AC_CANONICAL_HOST

    AS_CASE([x${host_os}],
        [xmingw*],      [nc_cv_platform_win32=yes],
        [xlinux*],      [nc_cv_platform_linux=yes],
        [xfreebsd*],    [nc_cv_platform_freebsd=yes],
        [AC_MSG_ERROR([unhandled  platform ${host_os}])]
    )

    AM_CONDITIONAL([PLATFORM_WIN32],    [test "x$nc_cv_platform_win32"   = "xyes"])
    AM_CONDITIONAL([PLATFORM_LINUX],    [test "x$nc_cv_platform_linux"   = "xyes"])
    AM_CONDITIONAL([PLATFORM_FREEBSD],  [test "x$nc_cv_platform_freebsd" = "xyes"])

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

        [freebsd*], [
            AX_APPEND_COMPILE_FLAGS([-I/usr/local/include])
            AX_APPEND_LINK_FLAGS([-L/usr/local/lib])
        ],

        [linux*], [:],

        [AC_MSG_ERROR([Unknown platform])]
    )

    AX_APPEND_COMPILE_FLAGS([-fno-common], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-fno-nonansi-builtins], [], [-Werror])
])
