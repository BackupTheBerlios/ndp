#include "ImplicitSurface3D.h"
#include <fstream>

ImplicitSurface3D::ImplicitSurface3D(ConstructRBFPOU::TypeRBF _type) {
  rbf = new ConstructRBFPOU(_type);
  b = g = r = new ConstructRBFPOU(_type);
  cs = new vector<Constraint>
  //  g = new ConstructRBFPOU(_type);
  //  b = new ConstructRBFPOU(_type);
}

ImplicitSurface3D::~ImplicitSurface3D() {
  delete cs;
  delete r,g,b;
  delete rbf;
}

ImplicitSurface3D::compute(PointSet &ps) {
  PointLst::const_iterator psBegin = ps.getBegin();
  PointList::const_iterator psEnd = filtered.getEnd();
  PointList::const_iterator i;
  
  for (i=psBegin; i!=psEnd; i++) {
    
  }
}

void ImplicitSurface3D::load(const std::string &filename) {
  std::ifstream stream(filename.c_str());
  
}

void ImplicitSurface3D::save(const std::string &filename) const {
  std::ofstream stream(filename.c_str());
  switch(type)
    {
    case BIHARMONIC:
      stream << 0 << endl;
      break;
    case TRIHARMONIC:
      stream << 1 << endl;
      break;
    case THINPLATE:
      stream << 2 << endl;
      break;
    }
  
  stream << threMin << " " << threMax << endl;
  
  unsigned int size = is.size();
  stream << size << endl;
  
  for(unsigned int i = 0; i<size; i++)
    {
      is[i]->save(stream);
    }
  
  for(unsigned int i = 0; i<size; i++)
    {
      (*cells)[i]->save(stream);
    }
}
