#include <vector>

#include "math/vector3.h"
#include "ImplicitSurface3D.h"
#include "mc.h"

using namespace std;

bool callback (int v, int max) {
  return true;
}


int main(int argc, char *argv) {
  Box3f    bbox;
  vector<Vec3f> vertices, normals;
  vector<unsigned int> indices;
  
//  ImplicitSurface3D is(ConstructRBFPOU::BIHARMONIC);
//  PointSet ps;
//  ps.load("dragon-soft.sur");
//  is.compute(ps, 3000);
//  is.save("test.pou");

  ImplicitSurface3D is;
  is.load("test.pou");
  Mc mc(callback, 10);
  mc.enableTet(true);
  mc.setCubeSize(0.03);
  mc.doMc(&is, bbox);
      
  return 0;
}
