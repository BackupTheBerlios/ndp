EXTRA_DIST=settings.ui
SUBDIRS=gl-headers mc

DEFS="-DDATADIR=\"${pkgdatadir}\""
INCLUDES=-I../$(top_srcdir)/libpou/src/

SUFFIXES = .ui
%.h: %.ui
	@UIC@ $< -o $@

%.cc: %.ui
	@UIC@ -impl $*.h $< -o $@

m%.cc: %.h
	@MOC@ $< -o $@

BUILT_SOURCES = settings.h settings.cc msettings.cc
bin_PROGRAMS=PoUgui
PoUgui_SOURCES=mwindow-moc.cc openglview.cc mwindow.cc\
	main.cc vertexbuffer.cc context.cc utils.cc xmlparser.cc\
	language.h mwindow.h openglview.h utils.h xmlparser.h\
	context.h opengl.h vertexbuffer.h trackball.cc trackball.h\
	light.h light.cc material.h material.cc\
	settings.ui.h settings.ui

nodist_PoUgui_SOURCES=settings.h settings.cc msettings.cc

PoUgui_CXXFLAGS=@QT_CFLAGS@ @GL_CFLAGS@
PoUgui_LDFLAGS=../$(top_srcdir)/libpou/src/libpou.la mc/mc.o
PoUgui_LDADD=@QT_LIBS@ @GL_LIBS@

clean-local:
	rm -f settings.h settings.cc msettings.cc

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
