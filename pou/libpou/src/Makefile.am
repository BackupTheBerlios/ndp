lib_LTLIBRARIES=libpou.la
libpou_la_SOURCES=AreaCube.cc AreaSetAdapt.cc AreaSet.cc AreaSetGrid.cc \
	AreaSetOctree.cc AreaSphere.cc PointSet.cc Solver.cc \
	ConstructRBF.cc ConstructRBFTriHarmonic.cc ConstructRBFBiHarmonic.cc


includedir=@includedir@/POU
include_HEADERS=Area.h AreaSetOctree.h ConstructRBFBiHarmonic.h box3d.h \
	AreaCube.h AreaSphere.h ConstructRBFTriHarmonic.h AreaSet.h \
	Constraint.h PointSet.h vector3.h AreaSetAdapt.h \
	ConstraintFilter.h Solver.h AreaSetGrid.h ConstructRBF.h W.h


libpou_la_CXXFLAGS=@GSL_CFLAGS@
libpou_la_LIBADD=@GSL_LIBS@
mostlyclean-local:
	@-rm -f *~
maintainer-clean-local:
	@-rm -f Makefile.in config.h.in stamp-h.in
