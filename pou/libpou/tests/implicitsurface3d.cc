#include "ImplicitSurface3D.h"

int main(int argc, char *argv) {
	ImplicitSurface3D is(ConstructRBFPOU::TRIHARMONIC);
	PointSet ps;
	ps.load("kameleon.sur");
	is.compute(ps, 3000);
	is.save("test.pou");
	return 0;
}
