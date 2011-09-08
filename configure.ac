AC_PREREQ(2.61)
AC_INIT([gentest], [0.1.3])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_HEADERS([config.h])
AC_CHECK_LIB(confuse, cfg_parse)
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_FUNCS(gettimeofday chdir chmod getopt fork waitpid)
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
