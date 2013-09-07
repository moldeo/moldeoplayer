ACLOCAL_AMFLAGS=-I m4

datadir = ${prefix}/share/moldeoplayer
modulesdir = ${libdir}/moldeoplayer
docdir = ${prefix}/share/doc/moldeoplayer
cursorsdir = ${datadir}/cursors
fontsdir = ${datadir}/fonts
iconsdir = ${datadir}/icons

AM_CPPFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

bin_PROGRAMS = moldeoplayerglfw

moldeoplayerglfw_SOURCES = main.cpp

moldeoplayerglfw_CPPFLAGS = \
    ${FT2_CFLAGS} \
	${GST_CFLAGS} \
	${GTK2_CFLAGS} \
	${MUPARSER_CFLAGS} \
	${BOOST_CPPFLAGS} \
	${SDL_CFLAGS} \
	${LUA51_CFLAGS} \ 
    ${LIBMOLDEO_CFLAGS}

#moldeoplayerglfw_LDFLAGS = -framework OpenGL -framework GLUT -framework Cocoa

moldeoplayerglfw_LDADD = \
   ${FT2_LIBS} \
	${GTK2_LIBS} \
	${SDL_LIBS} \
	${TINYXML_LIBS} \
	${GL_LIBS} \
	${GLEW_LIBS} \
	${FTGL_LIBS} \
	${GLUT_LIBS} \
	${GLU_LIBS} \
	${FREEIMAGE_LIBS} \
	${MUPARSER_LIBS} \
	${BOOST_ASIO_LIB} \
	${BOOST_THREAD_LIB} \
	${BOOST_FILESYSTEM_LIB} \
	${BOOST_SYSTEM_LIB} \
	${LUA51_LIBS} \
	${GST_LIBS} \
    ${LIBMOLDEO_LIBS}




