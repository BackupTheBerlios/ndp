#ifndef MC_H
#define MC_H

#include <vector>

#include "math/vector3.h"

void setcallback(void (*c)(int, int));
void domc(ImplicitSurface3D *imps, const Box3f &bbox);
void getVertNorm(std::vector<Vec3f> &vertices, std::vector<Vec3f> &normals);
const std::vector<unsigned int>& getIndices();


#endif
