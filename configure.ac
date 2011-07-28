AC_INIT([gentest], [0.0.9], [alessandro.rosetti@gmail.com], [gentest], [http://null.com/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])
AC_CHECK_LIB(confuse, cfg_parse)
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_FUNCS(gettimeofday chdir chmod)
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

