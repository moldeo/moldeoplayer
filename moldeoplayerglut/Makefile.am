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

bin_PROGRAMS = moldeoplayerglut

moldeoplayerglut_SOURCES = main.cpp moGLUT_IODeviceManager.h

moldeoplayerglut_CPPFLAGS = \
	${GST_CFLAGS} \
	${LUA51_CFLAGS} \
    ${LIBMOLDEO_CFLAGS}

#moldeoplayerglut_LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa

moldeoplayerglut_LDADD = \
	${GL_LIBS} \
	${GLEW_LIBS} \
	${GLUT_LIBS} \
	${GLU_LIBS} \
	${GST_LIBS} \
	${LUA51_LIBS} \
    ${LIBMOLDEO_LIBS}




