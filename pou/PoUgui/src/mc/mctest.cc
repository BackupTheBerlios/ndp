#include <vector>

#include "math/vector3.h"
#include "ImplicitSurface3D.h"
#include "mc.h"
using namespace std;


int main(int argc, char *argv) {
  vector<Vec3f> vertices, normals;
  vector<unsigned int> indices;
  
  ImplicitSurface3D is(ConstructRBFPOU::BIHARMONIC);
  PointSet ps;
  ps.load("dragon-soft.sur");
  is.compute(ps, 3000);
  is.save("test.pou");
//  ImplicitSurface3D is;
//  is.load("test.pou");
//   domc(&is, );
//   getVertNorm(vertices, normals);
//   indices = getIndices();
      
  return 0;
}
