#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(libhac, 0.2.5, bjoern@bjoern-b.de)
m4_define([package_major_version], [0])
m4_define([package_minor_version], [2])
m4_define([package_micro_version], [5])
m4_define([package_version], [package_major_version.package_minor_version.package_micro_version])
AM_INIT_AUTOMAKE(libhac, 0.2.5)
AC_CONFIG_SRCDIR([src/libhac.h])

LIBHAC_LIBTOOL_VERSION=1:0:0
AC_SUBST(LIBHAC_LIBTOOL_VERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([socket])
AC_OUTPUT(libhac.pc Makefile src/Makefile doc/Makefile doc/libhac.doxy)

