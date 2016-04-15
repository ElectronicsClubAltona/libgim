AC_DEFUN([NC_SUBPACKAGE_PC], [
    base=`basename $1 .pc`
    AC_CONFIG_FILES([$1:$base$nc_cv_config_pc_postfix.pc.in])
])


AC_DEFUN([NC_SUBPACKAGE_ENABLE],[
    AC_ARG_ENABLE([inline], [AS_HELP_STRING([--enable-inline], [enable building for use as an inline dependency])])

    AS_IF([test "x$enable_inline" == "xyes"], [
        AC_PROG_LN_S

        AC_CACHE_VAL([nc_cv_config_pc_postfix], [nc_cv_config_pc_postfix='-inline'])

        AC_CONFIG_COMMANDS([fake/cruft], [
            dnl make a fake include directory which looks like the typical
            dnl installation prefix. we should probably use AC_CONFIG_LINKS
            dnl but i'm not proficient enough at m4 to process the globs
            dnl correctly.

            AS_MKDIR_P(["fake/cruft"])
            $LN_S -nf "$ac_abs_top_srcdir" "fake/cruft/$cruft_base"

            AS_IF([test ! -L "fake/cruft/$cruft_base"], [
                AC_MSG_ERROR([could not create inline include directory])
            ])
        ], [
            cruft_base=`echo "${PACKAGE_NAME}" | sed 's/-.*//'`
        ])
    ])
])

