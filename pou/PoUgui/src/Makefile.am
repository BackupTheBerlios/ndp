EXTRA_DIST=settings.ui
SUBDIRS=gl-headers

DEFS="-DDATADIR=\"${pkgdatadir}\""

bin_PROGRAMS=PoUgui
PoUgui_SOURCES=mwindow-moc.cc openglview.cc mwindow.cc init.cc main.cc \
	vertexbuffer.cc
PoUgui_CXXFLAGS=@QT_CFLAGS@ @GL_CFLAGS@
PoUgui_LDFLAGS=../../libpou/src/libpou.la
PoUgui_LDADD=@QT_LIBS@ @GL_LIBS@

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in  stamp-h.in
