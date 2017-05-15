AC_INIT([Moldeo Player Sdl2], [1.0.0.0], [info@moldeointeractive.com.ar], [moldeoplayersdl2])

AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(config.h)
#AM_MAINTAINER_MODE

AC_LIBTOOL_CONFIG

AC_PROG_CC
AC_PROG_CC_STDC
AC_C_CONST
AC_PROG_LIBTOOL
AM_PROG_LIBTOOL

AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O

#dnl check for libmoldeo
PKG_CHECK_MODULES([LIBMOLDEO], [libmoldeo] ,has_libmoldeo=yes)

PKG_CHECK_MODULES([SDL2], [sdl2] ,has_sdl2=yes)

host_name="default"
host_cflags=""
host_ldflags=""
AC_ARG_ENABLE(host,
[--enable-host[=host name] Specify the host os name:
			64: 64 bit linux os [default]
			86: 32 bits linux os
            raspbian: RASPBIAN/debian 8 32 bits
            xosx: Mac OSX with mac ports],
[ if test "x$enableval" = "x86" ; then
	host_name="86"
  elif test "x$enableval" = "x64" ; then
	host_name="64"
  elif test "x$enableval" = "xraspbian" ; then
	host_name="raspbian"
	host_cflags="-I/opt/vc/include"
	host_ldflags="-L/opt/vc/lib"
  elif test "x$enableval" = "xosx" ; then
    host_name="osx"
	host_cflags="-I/opt/local/include "
	host_ldflags="-L/opt/local/lib -framework GLUT -framework OpenGL -framework Cocoa -framework GStreamer"
  else
	echo
	echo "Error!"
	echo "Unknown host name"
  fi
])

GST_VERSION_MAJOR=1
GST_VERSION_MINOR=0
GST_VERSION_RELEASE=0
GSTPLUG_VERSION_RELEASE=0    

AC_ARG_ENABLE(gstreamer,
[--enable-gstreamer[=gstreamer version] Specify the gstreamer version:
			1.0: Gstreamer version 1.0 [default]
			0.10: Gstreamer version 0.10],
[ if test "x$enableval" = "x0.10" ; then
	GST_VERSION_MAJOR=0
	GST_VERSION_MINOR=10
	GST_VERSION_RELEASE=19
	GSTPLUG_VERSION_RELEASE=7
  fi
])


if test "x$GST_VERSION_MAJOR" = "x1" ; then
	host_cflags="$host_cflags -DGSTVERSION"
fi

AC_SUBST([HOSTCFLAGS],$host_cflags)
AC_SUBST([HOSTLDFLAGS],$host_ldflags)

GSTREAMER_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GST_VERSION_RELEASE
GSTPLUGINS_REQ=$GST_VERSION_MAJOR.$GST_VERSION_MINOR.$GSTPLUG_VERSION_RELEASE
GST_MAJORMINOR=$GST_VERSION_MAJOR.$GST_VERSION_MINOR

#PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GSTREAMER_REQ,
#  HAVE_GSTREAMER=yes)
if test "x$host_name" = "xosx" ; then

AC_SUBST(GST_CFLAGS,"-I/Library/Frameworks/GStreamer.framework/Versions/1.0/Headers")
AC_SUBST(GST_LIBS,"-framework GStreamer")

else

PKG_CHECK_MODULES(GST, gstreamer-$GST_MAJORMINOR >= $GSTREAMER_REQ,
  HAVE_GSTREAMER=yes)
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)

fi


AC_SUBST(CPPFLAGS)
AC_SUBST(CXXFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(LIBS)

WX_GTK2_CFLAGS=
WX_GTK2_LIBS=


dnl autoscan results
AC_CHECK_FUNCS([floor])
AC_CHECK_FUNCS([getcwd])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([pow])
AC_CHECK_FUNCS([rint])
AC_CHECK_FUNCS([sqrt])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strstr])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([float.h])
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([stddef.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_TYPES([ptrdiff_t])
AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_VPRINTF
AC_HEADER_STDBOOL
AC_HEADER_STDC
AC_PREREQ
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
LT_INIT
AC_STRUCT_TM
AC_TYPE_SIZE_T

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes) debug=true ;;
             no)  debug=false ;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

echo "GSTREAMER VERSION: $GST_MAJORMINOR > $GSTREAMER_REQ"
echo "GST_CFLAGS: $GST_CFLAGS"
echo "GST_LIBS: $GST_LIBS"
echo "HOSTCFLAGS: $HOSTCFLAGS"
echo "HOSTLDFLAGS: $HOSTLDFLAGS"
echo "LIBMOLDEO_CFLAGS: $LIBMOLDEO_CFLAGS"
echo "LIBMOLDEO_LIBS: $LIBMOLDEO_LIBS"

dnl Makefile outputs
AC_OUTPUT([
Makefile
])

