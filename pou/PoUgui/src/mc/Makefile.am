INCLUDES=-I../$(top_srcdir)/libpou/src/
bin_PROGRAMS=mc
mc_SOURCES=mc.cc
	
mc_LDFLAGS=../$(top_srcdir)/libpou/src/libpou.la

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in  stamp-h.in
