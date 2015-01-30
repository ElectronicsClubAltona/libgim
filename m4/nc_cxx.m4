AC_DEFUN([NC_CXX],[
    AX_APPEND_COMPILE_FLAGS([-std=c++1y])

    AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list], [], [-Werror])
    AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list], [], [-Werror])

    AC_C_CONST
    AC_C_RESTRICT
    AC_C_INLINE

    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([struct A     { virtual void C (void) = 0;  };
                                        struct B : A { void C (void) override; }; ],
                                       [])],
                      [], [AC_DEFINE([override], [], [Pretend about override keyword support])])

    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([struct A final { }; ],
                                       [])],
                      [], [AC_DEFINE([final], [], [Pretend about final keyword support])])

])
