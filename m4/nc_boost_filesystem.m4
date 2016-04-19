AC_DEFUN([NC_BOOST_FILESYSTEM], [
    AC_REQUIRE([NC_BOOST_BASE])
    AC_REQUIRE([NC_BOOST_SYSTEM])

    AC_LANG_PUSH([C++])
        for lib in boost_filesystem; do
            AC_MSG_CHECKING([if boost filesystem library is $lib])

            nc_save_cflags=$CFLAGS
            nc_save_libs=$LIBS

            CFLAGS="$CFLAGS $BOOST_BASE_CPPFLAGS $BOOST_SYSTEM_CPPFLAGS"
            LIBS="$LIBS $BOOST_BASE_LDFLAGS $BOOST_SYSTEM_LDFLAGS -l$lib"

            dnl filesystem::path::filename appears to be extern. try calling it.
            AC_TRY_LINK(
                [@%:@include <boost/filesystem/path.hpp>],
                [boost::filesystem::path (".").filename ();],
                [nc_cv_boost_filesystem=yes],
                [nc_cv_boost_filesystem=no]
            )

            BOOST_FILESYSTEM_CPPFLAGS=$CFLAGS
            BOOST_FILESYSTEM_LDFLAGS=-l$lib
            
            CFLAGS=$nc_save_cflags
            LIBS=$nc_save_libs

            AC_MSG_RESULT([$nc_cv_boost_filesystem])
            AS_IF([test "x$nc_cv_boost_filesystem" = "xyes"], [break;])
        done
    AC_LANG_POP([C++])

    AS_IF([test "x$nc_cv_boost_filesystem" != "xyes"], [
        AC_MSG_ERROR([unable to locate a suitable boost_filesystem])
    ])
])
