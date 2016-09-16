AC_INIT([ldcopy], [0.2], [chrootlinux@krehel.sk])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CHECK_PROG(LDD_CHECK,ldd,yes)
if test x"$LDD_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Error!! Program ldd don't was found.])
fi
AC_CHECK_PROG(LN_CHECK,ln,yes)
if test x"$LN_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Error!! Program ln don't was found.])
fi
AC_CHECK_PROG(MKDIR_CHECK,mkdir,yes)
if test x"$MKDIR_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Error!! Program mkdir don't was found.])
fi
AC_CHECK_PROG(PCP_CHECK,cp,yes)
if test x"$PCP_CHECK" != x"yes" ; then
    AC_MSG_ERROR([Error!! Program cp don't was found.])
fi
AC_PROG_CC
AC_LANG_C
AC_CONFIG_HEADERS([config.h])
AC_FUNC_STRNLEN
AC_HEADER_STAT
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
