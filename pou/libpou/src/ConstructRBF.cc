#include "ConstructRBF.h"

ConstructRBF::ConstructRBF(): size(0), w(NULL), center(NULL), c(NULL) {
  cf=const_cast<ConstraintFilter*>(NULL_FILTER);
}

ConstructRBF::~ConstructRBF() {
  delete[] w;
  delete[] center;
}

void
ConstructRBF::evalNormal(const Vec3f &p, Vec3f &v) const
{ 
  evalGradian(p, v);
  v.normalize();  
}

const ConstraintFilter *ConstructRBF::NULL_FILTER = new NullFilter();
