EXTRA_DIST=settings.ui
SUBDIRS=gl-headers mc

DEFS="-DDATADIR=\"${pkgdatadir}\""
INCLUDES=-I../$(top_srcdir)/libpou/src/

bin_PROGRAMS=PoUgui
PoUgui_SOURCES=mwindow-moc.cc openglview.cc mwindow.cc init.cc \
	main.cc vertexbuffer.cc context.cc utils.cc\
	language.h mwindow.h openglview.h utils.h\
	context.h init.h opengl.h vertexbuffer.h\
	settings.h settings.cc msettings.cc settings.ui.h

#nodist_PoUgui_SOURCES=settings.h settings.cc msettings.cc

PoUgui_CXXFLAGS=@QT_CFLAGS@ @GL_CFLAGS@
PoUgui_LDFLAGS=../$(top_srcdir)/libpou/src/libpou.la mc/mc2.o
PoUgui_LDADD=@QT_LIBS@ @GL_LIBS@


%.h: %.ui
	@UIC@ $< -o $@

%.cc: %.ui
	@UIC@ -impl $*.h $< -o $@

m%.cc: %.h
	@MOC@ $< -o $@


clean-local:
	rm -f settings.h settings.cc msettings.cc

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
