AC_INIT([Moldeo Player Glut], [0.7.9.8], [info@moldeointeractive.com.ar], [moldeodirector])

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


#dnl check for sdl-net
#AC_CHECK_HEADER(SDL/SDL_net.h, , AC_MSG_ERROR([SDL_net header not found.]))
#AC_CHECK_LIB(SDL_net, SDLNet_Init, AC_SUBST([SDL_NET_LIBS], [-lSDL_net]), AC_MSG_ERROR([SDL_net not found.]), [${SDL_LIBS} ${WIN32_LIBS}])

#dnl check for glew
#AC_CHECK_HEADER(GL/glew.h, , AC_MSG_ERROR([glew header not found.]))
#AC_CHECK_LIB(GLEW, glewInit, AC_SUBST([GLEW_LIBS], [-lGLEW]), AC_MSG_ERROR([glew not found.]), )

#dnl check for glut
AC_CHECK_HEADER(GL/glut.h, , AC_MSG_ERROR([glut header not found.]))
AC_CHECK_LIB(glut, glutMainLoop, AC_SUBST([GLUT_LIBS], [-lglut]), AC_MSG_ERROR([glut not found.]), )

#dnl check for gle
#AC_CHECK_HEADER(GL/gle.h, , AC_MSG_ERROR([gle header not found.]))
#AC_CHECK_LIB(gle, gleDestroyGC, AC_SUBST([GLE_LIBS], [-lgle]), AC_MSG_ERROR([gle not found.]), )

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

dnl Makefile outputs
AC_OUTPUT([
Makefile
])

