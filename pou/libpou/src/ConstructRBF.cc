#include "ConstructRBF.h"

ConstructRBF::ConstructRBF() {
  size = 0;
  center = 0;
  w = 0;
  c = 0;
  cf=const_cast<ConstraintFilter*>(NULL_FILTER);
}

ConstructRBF::~ConstructRBF() {
  /*  if (w)
    delete[] w;
  if (center)
    delete[] center;
  if (c)
  delete[] c;*/
}

void
ConstructRBF::evalNormal(const Vec3f &p, Vec3f &v) const
{ 
  evalGradian(p, v);
  v.normalize();  
}

const ConstraintFilter *ConstructRBF::NULL_FILTER = new NullFilter();
