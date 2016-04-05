AC_DEFUN([NC_BOOST_THREAD], [
    AC_REQUIRE([NC_BOOST_BASE])
    AC_REQUIRE([NC_BOOST_SYSTEM])

    AC_REQUIRE([AC_CANONICAL_HOST])
    AS_CASE(["x$host_os"],
        ["*xmingw*"], [
            extra_cflags="-mthreads"
        ], [
            AC_REQUIRE([AX_PTHREAD])
            extra_cflags="$PTHREAD_CFLAGS"
            extra_libs="$PTHREAD_LIBS"
        ]
    )

    AC_LANG_PUSH([C++])
        for lib in boost_thread boost_thread_win32; do
            AC_MSG_CHECKING([if boost threading library is $lib])

            nc_save_cflags=$CFLAGS
            nc_save_libs=$LIBS

            CFLAGS="$CFLAGS $BOOST_BASE_CPPFLAGS $BOOST_SYSTEM_CPPFLAGS $extra_cflags"
            LIBS="$LIBS $BOOST_BASE_LDFLAGS $BOOST_SYSTEM_LDFLAGS $extra_libs -l$lib"

            dnl Try to call this_thread::yield as it appears to be extern
            AC_TRY_LINK(
                [@%:@include <boost/thread/thread.hpp>],
                [boost::this_thread::yield ();],
                [nc_cv_boost_thread=yes],
                [nc_cv_boost_thread=no]
            )

            BOOST_THREAD_CPPFLAGS=$CFLAGS
            BOOST_THREAD_LDFLAGS=$LIBS

            CFLAGS=$nc_save_cflags
            LIBS=$nc_save_libs

            AC_MSG_RESULT([$nc_cv_boost_thread])
            AS_IF([test "x$nc_cv_boost_thread" = "xyes"], [break;])
        done
    AC_LANG_POP([C++])

    AS_IF([test "x$nc_cv_boost_thread" != "xyes"], [
        AC_MSG_ERROR([unable to locate a suitable boost_thread])
    ])
])
