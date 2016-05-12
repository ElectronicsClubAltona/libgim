AC_DEFUN([NC_BOOST_SYSTEM], [
    AC_REQUIRE([NC_BOOST_BASE])

    AC_LANG_PUSH([C++])
        for lib in boost_system; do
            AC_MSG_CHECKING([if boost system library is $lib])

            nc_save_cxxflags=$CXXFLAGS
            nc_save_libs=$LIBS

            CXXFLAGS="$CXXFLAGS $BOOST_BASE_CPPFLAGS"
            LIBS="$LIBS $BOOST_BASE_LDFLAGS -l$lib"

            dnl Try to call boost::system::throws as it appears to be extern
            AC_TRY_LINK(
                [@%:@include <boost/system/error_code.hpp>],
                [boost::system::throws ();],
                [nc_cv_boost_system=yes],
                [nc_cv_boost_system=no]
            )

            BOOST_SYSTEM_CPPFLAGS=$CXXFLAGS
            BOOST_SYSTEM_LDFLAGS=-l$lib

            CXXFLAGS=$nc_save_cxxflags
            LIBS=$nc_save_libs

            AC_MSG_RESULT([$nc_cv_boost_system])
            AS_IF([test "x$nc_cv_boost_system" = "xyes"], [break;])
        done
    AC_LANG_POP([C++])

    AS_IF([test "x$nc_cv_boost_system" != "xyes"], [
        AC_MSG_ERROR([unable to locate boost_system])
    ])
])
