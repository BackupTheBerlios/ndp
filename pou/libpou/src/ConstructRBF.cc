/**
 * @file   ConstructRBF.cc
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:52:36 2004
 * 
 * @brief  Radial Basis Functions
 *
 * $Log: ConstructRBF.cc,v $
 * Revision 1.7  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.6  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
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
  evalGradient(p, v);
  v.normalize();  
}

const ConstraintFilter *ConstructRBF::NULL_FILTER = new NullFilter();
