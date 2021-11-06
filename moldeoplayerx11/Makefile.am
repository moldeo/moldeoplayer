ACLOCAL_AMFLAGS=-I m4

if DEBUG
DFLAGS = -g3 -O0 -fpermissive -DMODEBUG
else
DFLAGS = -O2 -fpermissive
endif

datadir = ${prefix}/share/moldeoplayer
modulesdir = ${libdir}/moldeoplayer
docdir = ${prefix}/share/doc/moldeoplayer
cursorsdir = ${datadir}/cursors
fontsdir = ${datadir}/fonts
iconsdir = ${datadir}/icons

AM_CPPFLAGS = ${DFLAGS} ${HOSTCFLAGS} $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = ${AM_CPPFLAGS}
AM_LDFLAGS = ${HOSTLDFLAGS}

bin_PROGRAMS = moldeoplayerx11

moldeoplayerx11_SOURCES = AUTHORS README NEWS ChangeLog main.cpp moX11_IODeviceManager.h EasyTab/easytab.h

moldeoplayerx11_CPPFLAGS = \
	${GST_CFLAGS} \
	${X11_CFLAGS} \
  ${XINERAMA_CFLAGS} \
	${XINPUT_CFLAGS} \
	${GL_CFLAGS} \
	${LUA51_CFLAGS} \
    ${LIBMOLDEO_CFLAGS}

#moldeoplayerx11_LDFLAGS = -framework OpenGL -framework SDL2 -framework Cocoa

moldeoplayerx11_LDADD = \
	${X11_LIBS} \
  ${XINERAMA_LIBS} \
	${XINPUT_LIBS} \
	${GL_LIBS} \
	${GLUT_LIBS} \
	${LUA51_LIBS} \
	${GST_LIBS} \
    ${LIBMOLDEO_LIBS}
