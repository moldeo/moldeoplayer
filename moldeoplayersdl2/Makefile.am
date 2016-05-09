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

bin_PROGRAMS = moldeoplayersdl2

moldeoplayersdl2_SOURCES = main.cpp moSDL_IODeviceManager.h

moldeoplayersdl2_CPPFLAGS = \
	${GST_CFLAGS} \
	${SDL2_CFLAGS} \
	${GL_CFLAGS} \
	${LUA51_CFLAGS} \
    ${LIBMOLDEO_CFLAGS}

#moldeoplayersdl2_LDFLAGS = -framework OpenGL -framework SDL2 -framework Cocoa

moldeoplayersdl2_LDADD = \
	${SDL2_LIBS} \
	${GL_LIBS} \
	${GLUT_LIBS} \
	${LUA51_LIBS} \
	${GST_LIBS} \
    ${LIBMOLDEO_LIBS}




