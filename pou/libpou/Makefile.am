AUTOMAKE_OPTIONS=dist-bzip2
ACLOCAL=aclocal -I m4
SUBDIRS=src tests

m4datadir=@datadir@/aclocal
m4data_DATA=pou.m4
EXTRA_DIST=pou.m4

bin_SCRIPTS=pou-config

doc:
	doxygen
mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f bootstrap.log configure Makefile.in missing config.guess
	@-rm -f mkinstalldirs Doxyfile config.log INSTALL aclocal.m4
	@-rm -f config.status install-sh Makefile config.sub ltmain.sh
	@-rm -fr autom4te.cache
	@-rm -f COPYING depcomp pou-config
