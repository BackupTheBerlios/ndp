#include "ImplicitSurface3D.h"

int main(int argc, char *argv) {
	ImplicitSurface3D is;
	PointSet ps;
	ps.load("dragon-soft.sur");
	is.compute(ps);
	return 0;
}
