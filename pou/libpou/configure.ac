dnl Process this file with autoconf to produce a configure script.
AC_INIT([libpou], [0.1], [http://developer.berlios.de/bugs/?func=addbug&group_id=1564])

CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"

dnl Setup for automake
AC_CONFIG_SRCDIR([src/AreaCube.cc])

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Setup for automake
AM_INIT_AUTOMAKE([$PACKAGE_NAME], [$PACKAGE_VERSION])
AM_CONFIG_HEADER([src/config.h])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX

AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LN_S
AC_C_INLINE

AC_LANG([C++])

dnl Checks for header files.
AC_HEADER_STDC

dnl Enable libtool without static lib build
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_CHECK_LIB(z, gzopen, Z_LIBS="-lz", AC_MSG_WARN([*** libz not found. Cannot manage compressed files ***]))

AC_SUBST(Z_LIBS)

AC_CHECK_HEADER([ext/hash_set], :, AC_MSG_ERROR([*** cannot find hash_set header ***]))

AM_PATH_GSL(1.1, :, AC_MSG_ERROR([*** libgsl not found ***]))

dnl Modify CFLAGS after all tests
dnl Add args to configure
AC_ARG_ENABLE(debug,   [  --disable-debug          Enable debugging information],
        USE_DEBUG="no", USE_DEBUG="yes")

AC_ARG_ENABLE(optimise,[  --enable-optimise       Enable code optimising],
	USE_OPTIMISE="$enableval", USE_OPTIMISE="no")

AC_ARG_ENABLE(profile, [  --enable-profile        Enable code profiling],
	USE_PROFILE="$enableval", USE_PROFILE="no")

dnl Flags
AC_TRY_CFLAGS("-Wall", wall="-Wall", wall="")
CFLAGS="$CFLAGS $wall"
CXXFLAGS="$CXXFLAGS $wall"
dnl Support for debug
if test "$USE_DEBUG" = yes
then
	CFLAGS="$CFLAGS -g"
	CXXFLAGS="$CXXFLAGS -g -DDEBUG"
fi

dnl Support for profiling
if test "$USE_PROFILE" = yes
then
	CFLAGS="$CFLAGS -pg"
	CXXFLAGS="$CXXFLAGS -pg"
fi

dnl Support for optimize
if test "$USE_OPTIMIZE" = yes
then
	CFLAGS="$CFLAGS -O3"
	CXXFLAGS="$CXXFLAGS -O3"
	LDFLAGS="$LDFLAGS -s"
fi

dnl Warning as error
AC_ARG_ENABLE(werror, [  --enable-werror                Make all warnings into errors], CFLAGS="$CFLAGS -Werror" CXXFLAGS="$CXXFLAGS -Werror",:)


dnl ---------------------------------------------
dnl It seems automake 1.5 don't take care about this script
dnl ---------------------------------------------
if test ! -z "$am_depcomp"; then
        DEPCOMP="depcomp"
fi
AC_SUBST(DEPCOMP)

AC_OUTPUT([pou-config Makefile src/Makefile tests/Makefile Doxyfile])
