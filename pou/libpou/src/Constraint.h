/**
 * @file   Constraint.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:45:59 2004
 * 
 * @brief  One Constraint management
 *
 * $Log: Constraint.h,v $
 * Revision 1.5  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.4  2004/04/07 08:02:30  leserpent
 * Added a	default value to the parameter param of the constructor
 *
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <ostream>
#include "math/vector3.h"

class Constraint {
  Vec3f v;
  float c;
 public:
  Constraint(const Vec3f& _v, const float _c = 0) : v(_v), c(_c) {}

  Vec3f getVector() const {
    return v;
  }
  
  float getConstraint() const {
    return c;
  }
  void setVector(const Vec3f &_v) {
    v=_v;
  }
  void setConstraint(const float _c) {
    c=_c;
  }
  friend std::ostream& operator<< (std::ostream &os, const Constraint &c) {
    os << "<(" << c.v << ")," << c.c << ">";
    return os;
  }
};

#endif /* CONSTRAINT_H */
