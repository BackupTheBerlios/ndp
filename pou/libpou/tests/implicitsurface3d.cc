#include "ImplicitSurface3D.h"

int main(int argc, char *argv) {
	ImplicitSurface3D is;
	PointSet ps, psF;
	ps.load("dragon-soft.sur");
        psF = PointSet(ps, 3000);
	is.compute(psF);
	return 0;
}
