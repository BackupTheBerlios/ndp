TESTS=constructrbf implicitsurface3d quaterniontest #osg-compatible
XFAIL_TESTS= # tests devant echouer
noinst_PROGRAMS=$(TESTS)


INCLUDES=-I../modules
#osg_compatible_SOURCES=osg-compatible.cc apply.h equal.h
#osg_compatible_CXXFLAGS=$(shell osg-config --cflags) -O
#osg_compatible_LDFLAGS=$(shell osg-config --libs)
quaterniontest_SOURCES=quaterniontest.cc
constructrbf_SOURCES=constructrbf.cc
LDADD=../src/libpou.la
implicitsurface3d_SOURCES=implicitsurface3d.cc


mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in
