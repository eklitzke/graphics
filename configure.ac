AC_PREREQ([2.65])
AC_INIT([graphics], [0.1], [evan@eklitzke.org])
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL

AC_CHECK_LIB(m, main, [], [AC_MSG_ERROR([missing m])])
AC_CHECK_LIB(glut, main, [], [AC_MSG_ERROR([missing glut])])
AC_CHECK_LIB(GLEW, main, [], [AC_MSG_ERROR([missing GLEW])])
AC_CHECK_LIB(GL, main, [], [AC_MSG_ERROR([missing GL])])

# options
AC_ARG_ENABLE(debug,
	      [ --enable-debug     build with tons of debugging],
	      [if test x$enableval = xyes ; then
		  AC_DEFINE([DEBUG], 1, [debugging enabled])
	       fi]
	     )

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
