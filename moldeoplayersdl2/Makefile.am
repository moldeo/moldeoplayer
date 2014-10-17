ACLOCAL_AMFLAGS=-I m4

datadir = ${prefix}/share/moldeoplayer
modulesdir = ${libdir}/moldeoplayer
docdir = ${prefix}/share/doc/moldeoplayer
cursorsdir = ${datadir}/cursors
fontsdir = ${datadir}/fonts
iconsdir = ${datadir}/icons

AM_CPPFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\" -fpermissive
AM_CXXFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\" -fpermissive

bin_PROGRAMS = moldeoplayersdl2

moldeoplayersdl2_SOURCES = main.cpp moSDL_IODeviceManager.h

moldeoplayersdl2_CPPFLAGS = \
    ${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GTK2_CFLAGS} \
	${BOOST_CPPFLAGS} \
	${SDL2_CFLAGS} \
	${LUA51_CFLAGS} \ 
    ${LIBMOLDEO_CFLAGS}

#moldeoplayersdl2_LDFLAGS = -framework OpenGL -framework SDL2 -framework Cocoa

moldeoplayersdl2_LDADD = \
   ${FT2_LIBS} \
	${GTK2_LIBS} \
	${SDL2_LIBS} \
	${GL_LIBS} \
	${GLEW_LIBS} \
	${FTGL_LIBS} \
	${GLUT_LIBS} \
	${GLU_LIBS} \
	${LUA51_LIBS} \
	${GST_LIBS} \
    ${LIBMOLDEO_LIBS}




