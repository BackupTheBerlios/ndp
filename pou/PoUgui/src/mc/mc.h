#ifndef MC_H
#define MC_H

#include <vector>

void domc(ImplicitSurface3D *imps);
void getVertNorm(std::vector<double> &vertices, std::vector<double> &normals);
const std::vector<int>& getIndices();



#endif
