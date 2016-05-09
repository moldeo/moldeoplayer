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

bin_PROGRAMS = moldeoplayersdl

moldeoplayersdl_SOURCES = main.cpp moSDL_IODeviceManager.h

moldeoplayersdl_CPPFLAGS = \
    ${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GTK2_CFLAGS} \
	${MUPARSER_CFLAGS} \
	${SDL_CFLAGS} \
	${LUA51_CFLAGS} \
    ${LIBMOLDEO_CFLAGS}

#moldeoplayersdl_LDFLAGS = -framework OpenGL -framework SDL -framework Cocoa

moldeoplayersdl_LDADD = \
   ${FT2_LIBS} \
	${GTK2_LIBS} \
	${SDL_LIBS} \
	${TINYXML_LIBS} \
	${GL_LIBS} \
	${GLEW_LIBS} \
	${LUA51_LIBS} \
	${GST_LIBS} \
    ${LIBMOLDEO_LIBS}




