#ifndef CONSTRUCTRBF_H
#define CONSTRUCTRBF_H

#include <vector>
#include <fstream>
#include "Constraint.h"
#include "ConstraintFilter.h"
#include "vector3.h"

class NullFilter : public ConstraintFilter {
 public:
  void filter(std::vector<Constraint>& cs, int parm) {
  }
};


class ConstructRBF {
  ConstraintFilter *cf;
 public:
  ConstructRBF();
  virtual ~ConstructRBF();
  virtual int compute(const std::vector<Constraint> &cs) = 0;
  virtual float eval(const Vec3f &p) const = 0;
  void evalNormal(const Vec3f &p, Vec3f &v) const;
  virtual void evalGradian(const Vec3f &p, Vec3f &v) const =0;

  virtual void load(std::ifstream &stream) = 0;
  virtual void save(std::ofstream &stream) const = 0;


  unsigned int getSize();
  float getW(unsigned int index) const;

  Vec3f& getCenter(const unsigned int index) const;

  void setSize(const unsigned int _size);
  void setW(const unsigned int index, const float val);
  void setCenter(const unsigned int index, const Vec3f& _center);


 protected:
  unsigned int size;
  float *w;
  Vec3f *center;
  float *c;

  virtual double phi(const double r) const = 0;
  virtual double dist(const Vec3f &p1, const Vec3f &p2) const {
    return (p1 - p2).length();
  }

};

inline unsigned int ConstructRBF::getSize() {
  return size;
}

inline float ConstructRBF::getW(unsigned int index) const {
    return w[index];
}

inline Vec3f& ConstructRBF::getCenter(const unsigned int index) const {
  return center[index];
}

inline void ConstructRBF::setSize(const unsigned int _size) {
  size = _size;
  if (w)
    delete[] w;
  if (center)
    delete[] center;
  
  w = new float[size];
  center = new Vec3f[size];
}

inline void ConstructRBF::setW(const unsigned int index, const float val) {
  w[index] = val;
}

inline void ConstructRBF::setCenter(const unsigned int index, const Vec3f& _center) {
  center[index] = _center;
}

#endif /* CONSTRUCTRBF_H */
