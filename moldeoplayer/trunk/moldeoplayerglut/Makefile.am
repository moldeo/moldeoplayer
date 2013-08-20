ACLOCAL_AMFLAGS=-I m4

datadir = ${prefix}/share/moldeoplayer
modulesdir = ${libdir}/moldeoplayer
docdir = ${prefix}/share/doc/moldeoplayer
cursorsdir = ${datadir}/cursors
fontsdir = ${datadir}/fonts
iconsdir = ${datadir}/icons

AM_CPPFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"
AM_CXXFLAGS = $(all_includes) -DMOLDEODATADIR=\"$(datadir)\" -DMODULESDIR=\"$(modulesdir)\"

bin_PROGRAMS = moldeoplayerglut

moldeoplayerglut_SOURCES = main.cpp

moldeoplayerglut_CPPFLAGS = \
	${LIBMOLDEO_CFLAGS} 

moldeoplayerglut_LDADD = \
	${LIBMOLDEO_LIBS} \
	${GLUT_LIBS}




