lib_LTLIBRARIES=libpou.la
libpou_la_SOURCES=AreaCube.cc AreaSet.cc \
	AreaSetOctree.cc AreaSphere.cc PointSet.cc Solver.cc \
	ConstructRBF.cc ConstructRBFTriHarmonic.cc ConstructRBFBiHarmonic.cc \
	ConstructRBFPOU.cc ImplicitSurface3D.cc ConstraintSet.cc \
	helpers/md5.c helpers/gzstream.C

pkginclude_HEADERS=Area.h AreaSetOctree.h ConstructRBFBiHarmonic.h box3d.h \
	AreaCube.h AreaSphere.h ConstructRBFTriHarmonic.h \
	ConstructRBFThinPlate.h AreaSet.h Constraint.h PointSet.h \
	ConstraintFilter.h Solver.h ConstructRBF.h W.h \
	ConstructRBFPOU.h ImplicitSurface3D.h ConstraintSet.h Md5.h

pkgincludemathdir=@includedir@/@PACKAGE_NAME@
pkgincludemath_HEADERS=math/vector3.h math/quaternion.h math/matrix4.h \
	math/matrix.h

pkgincludehelpersdir=@includedir@/@PACKAGE_NAME@
pkgincludehelpers_HEADERS=helpers/md5.h helpers/deletor.h helpers/gzstream.h


libpou_la_CXXFLAGS=@GSL_CFLAGS@
libpou_la_LIBADD=@GSL_LIBS@ @Z_LIBS@
mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
