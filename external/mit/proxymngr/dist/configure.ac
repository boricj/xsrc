
dnl  Copyright 2005 Red Hat, Inc.
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Red Hat not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Red Hat makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  RED HAT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL RED HAT BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.60])
AC_INIT([proxymngr], [1.0.2],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=xorg], [proxymngr])
AM_INIT_AUTOMAKE([dist-bzip2])
AM_MAINTAINER_MODE

# Require X.Org macros 1.8 or later for MAN_SUBSTS set by XORG_MANPAGE_SECTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.8 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.8)
XORG_DEFAULT_OPTIONS

# The program itself has a header called config.h so 
# we have to choose a different name
AM_CONFIG_HEADER(xconfig.h)

AC_PROG_CPP

AC_PATH_PROG(LBXPROXY, lbxproxy)
if [[ -z $LBXPROXY ]] ; then
   AC_MSG_ERROR([lbxproxy is needed])
fi

# Checks for pkg-config packages
PKG_CHECK_MODULES(PROXYMNGR, ice xt x11 xproxymngproto)

AC_CHECK_FUNC(strcasecmp, [], 
	AC_DEFINE([NEED_STRCASECMP],1,
		  [Define to 1 if you do not have the "strcasecmp" function.]))

# Needed to check for IPv6 support and set flags appropriately
XTRANS_TCP_FLAGS

AC_OUTPUT([Makefile])
