AC_PREREQ(2.61)
AC_INIT([gentest], [0.1.3])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_HEADERS([config.h])

AC_CHECK_HEADERS([signal.h fcntl.h locale.h stdint.h stdlib.h unistd.h])
AC_CHECK_FUNCS(gettimeofday chdir chmod getopt fork waitpid)

AC_CHECK_LIB(confuse, cfg_parse)
AC_CHECK_LIB(pthread, pthread_create)

DISTCLEANFILES="Makefile.in"
AC_SUBST(DISTCLEANFILES)

AC_PROG_CXX
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
