AC_PREREQ(2.61)
AC_INIT([gentest], [0.2.5])
AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_HEADERS([config.h])

AC_CHECK_HEADERS([signal.h fcntl.h locale.h stdint.h stdlib.h unistd.h])
AC_CHECK_FUNCS(gettimeofday chdir chmod getopt fork waitpid)

AC_CHECK_LIB(confuse, cfg_parse)
AC_CHECK_LIB(pthread, pthread_create)
#AC_CHECK_LIB(boost_random, normal_distribution)

#profile
AC_ARG_ENABLE(
profile,
[ --enable-profile=ARG enable support for profiling (default=no) ],
[enable_profile=$enableval],
[enable_profile=no]
)
if test "$enable_profile" = "yes"; then
CXXFLAGS="$CXXFLAGS -pg"
AC_DEFINE(ENABLE_PROFILE,1,[profile option])
else
AC_DEFINE(ENABLE_PROFILE,1,[profile option])
fi

#debug
AC_ARG_ENABLE(
debug,
[--enable-debug=ARG enable support for debugging (default=no)],
[enable_debug=$enableval],
[enable_debug=no]
)
if test "$enable_debug" = "yes"; then
CXXFLAGS="-O0 -g -Wall -Werror -pedantic"
AC_DEFINE(ENABLE_DEBUG,1,[debug option])
else
AC_DEFINE(ENABLE_DEBUG,0,[debug option])
fi

#release
AC_ARG_ENABLE(
release,
[--enable-release=ARG enable release version (default=no)],
[enable_release=$enableval],
[enable_release=yes]
)
if test "$enable_release" = "yes"; then
CXXFLAGS="-O3 -mtune=native -march=native -pipe -fomit-frame-pointer"
AC_DEFINE(ENABLE_RELEASE,1,[release option])
else
AC_DEFINE(ENABLE_RELEASE,0,[release option])
fi


AC_ARG_ENABLE(
mersenne_twister,
[--enable-mersenne_twister=ARG enable release version (default=yes)],
[enable_mersenne_twister=$enableval],
[enable_mersenne_twister=no]
)
if test "$enable_mersenne_twister" = "yes"; then
    AC_DEFINE(USE_MERSENNE_TWISTER,1,[ Mersenne Twister ])
fi

#CXXFLAGS="$CXXFLAGS -std=c++0x"

DISTCLEANFILES="Makefile.in"
AC_SUBST(DISTCLEANFILES)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
