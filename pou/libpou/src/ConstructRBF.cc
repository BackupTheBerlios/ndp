/**
 * @file   ConstructRBF.cc
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:52:36 2004
 * 
 * @brief  Radial Basis Functions
 *
 * $Log: ConstructRBF.cc,v $
 * Revision 1.5  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
#include "ConstructRBF.h"

ConstructRBF::ConstructRBF(): size(0), w(0), center(0), c(0) {
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
