#include "ImplicitSurface3D.h"

#include <fstream>

ImplicitSurface3D::ImplcitSurface3D(ConstructRBFPOU::TypeRBF _type) {
  rbf = new ConstructRBFPOU(_type);
}
ImplicitSurface3D::~ImplicitSurface3D() {
  delete rbf;
}

void ImplicitSurface3D::load(std::string filename) {
  ifstream stream(filename);
  
}

void ImplicitSurface3D::save(std::string filename) {
  ofstream stream(filename);
  
}
