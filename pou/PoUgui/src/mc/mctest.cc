#include "ImplicitSurface3D.h"

extern void domc(ImplicitSurface3D *imps);


int main(int argc, char *argv) {
// 	ImplicitSurface3D is(ConstructRBFPOU::BIHARMONIC);
// 	PointSet ps;
// 	ps.load("dragon-soft.sur");
// 	is.compute(ps, 3000);
// 	is.save("test.pou");
 	ImplicitSurface3D is;
        is.load("test.pou");
        domc(&is);
        
	return 0;
}
