#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <ostream>
#include "vector3.h"

class Constraint {
  Vec3f v;
  float c;
 public:
  Constraint(const Vec3f& _v, const float _c) : v(_v), c(_c) {}

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
