dnl Attempt to locate boost libraries.
dnl
dnl We explicitly do not use ax_boost_* macros aside from the version checks
dnl because they are catastrophically broken for any remotely complex setup,
dnl and I will just end up punching someone if I have to look at any more
dnl broken M4.
dnl
dnl $1 version required
dnl $2 space delimited list of components required
dnl
dnl sets BOOST_LDFLAGS and BOOST_CPPFLAGS

AC_DEFUN([NC_BOOST], [
    AC_REQUIRE([NC_BOOST_BASE])

    for _nc_boost_lib in $2; do
        AS_CASE([$_nc_boost_lib],
            ["system"],     [NC_BOOST_SYSTEM],
            ["thread"],     [NC_BOOST_THREAD],
            ["filesystem"], [NC_BOOST_FILESYSTEM],
            [AC_MSG_ERROR([unknown boost library $_nc_boost_lib])]
        )

        dnl find what the variable name is for this package's CPPFLAGS/LIBS
        _NAME="$(echo $_nc_boost_lib | tr "[a-z]" "[A-Z]")"
        _CPPFLAGS_NAME="BOOST_${_NAME}_CPPFLAGS"
        _LDFLAGS_NAME="BOOST_${_NAME}_LDFLAGS"

        dnl extract the aforementioned variable's values
        _CPPFLAGS="$(eval "echo \$$_CPPFLAGS_NAME")"
        _LDFLAGS="$(eval "echo \$$_LDFLAGS_NAME")"

        dnl append flags and libs if they don't already exist
        for flag in $_CPPFLAGS; do
            AS_IF([test " $BOOST_CPPFLAGS " != " *$flag* "], [BOOST_CPPFLAGS="$BOOST_CPPFLAGS $flag"])
        done

        for lib in $_LDFLAGS; do
            AS_IF([test " $BOOST_LDFLAGS " != " *$lib* "], [BOOST_LDFLAGS="$BOOST_LDFLAGS $lib"])
        done
    done
])
