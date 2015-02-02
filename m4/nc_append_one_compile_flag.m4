# SYNOPSIS
#
#   NC_APPEND_ONE_COMPILE_FLAG([FLAG1 FLAG2 ...], [FLAGS-VARIABLE], [EXTRA-FLAGS])
#
# DESCRIPTION
#
#   Append the first valid compilation flag in the provided list to FLAGS-VARIABLE


AC_DEFUN([NC_APPEND_ONE_COMPILE_FLAG], [
    AX_REQUIRE_DEFINED([AX_CHECK_COMPILE_FLAG])
    AX_REQUIRE_DEFINED([AX_APPEND_FLAG])

    for __dummy in "1"; do
        for flag in $1; do
            AX_CHECK_COMPILE_FLAG([$flag],
                                  [AX_APPEND_FLAG([$flag], [$2])
                                   break 2],
                                  [],
                                  [$3])
        done

        AC_MSG_ERROR([unable to find a valid compilation flag in '$1'])
    done
])
