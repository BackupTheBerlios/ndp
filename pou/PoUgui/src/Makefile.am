EXTRA_DIST=settings.ui
SUBDIRS=gl-headers mc

DEFS="-DDATADIR=\"${pkgdatadir}\""
INCLUDES=-I../$(top_srcdir)/libpou/src/

bin_PROGRAMS=PoUgui
PoUgui_SOURCES=mwindow-moc.cc openglview.cc mwindow.cc init.cc main.cc \
	vertexbuffer.cc context.cc settings.cc msettings.cc \
	ims.h   language.h  mwindow.h  openglview.h \
	context.h  init.h  libgui.h    opengl.h   vertexbuffer.h \
	settings.h

PoUgui_CXXFLAGS=@QT_CFLAGS@ @GL_CFLAGS@
PoUgui_LDFLAGS=../$(top_srcdir)/libpou/src/libpou.la mc/mc.o
PoUgui_LDADD=@QT_LIBS@ @GL_LIBS@


%.h: %.ui
	uic $< -o $@

%.cc: %.ui
	uic -impl $*.h $< -o $@

m%.cc: %.h
	moc $< -o $@
		   

clean-local:
	rm -f settings.h settings.cc msettings.cc

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in  stamp-h.in
