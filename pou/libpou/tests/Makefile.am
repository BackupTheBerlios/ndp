TESTS=constructrbf #osg-compatible
XFAIL_TESTS= # tests devant echouer
noinst_PROGRAMS=$(TESTS)


INCLUDES=-I../modules
#osg_compatible_SOURCES=osg-compatible.cc
#osg_compatible_CXXFLAGS=$(shell osg-config --cflags)
#osg_compatible_LDFLAGS=$(shell osg-config --libs)
constructrbf_SOURCES=constructrbf.cc
constructrbf_LDADD=../src/libpou.la

mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in
