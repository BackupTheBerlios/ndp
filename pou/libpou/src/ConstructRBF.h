/**
 * @file   ConstructRBF.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:44:49 2004
 * 
 * @brief  Radial Basis Functions
 * 
 * $Log: ConstructRBF.h,v $
 * Revision 1.8  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.7  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.6  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef CONSTRUCTRBF_H
#define CONSTRUCTRBF_H

#include <vector>
#include <fstream>
#include "Constraint.h"
#include "ConstraintSet.h"
#include "ConstraintFilter.h"
#include "math/vector3.h"

#include <iostream>


class NullFilter : public ConstraintFilter {
 public:
  void filter(ConstraintSet& cs, int parm) {
  }
};


class ConstructRBF {
 public:
  ConstructRBF();
  virtual ~ConstructRBF();

  int compute(const ConstraintSet &cs) {
    applyFilter(const_cast<ConstraintSet&>(cs));
    return computeRBF(cs);
  }

  virtual float eval(const Vec3f &p) const = 0;
  void evalNormal(const Vec3f &p, Vec3f &v) const;
  virtual void evalGradient(const Vec3f &p, Vec3f &v) const =0;

  virtual void load(std::istream &stream) = 0;
  virtual void save(std::ostream &stream) const = 0;


  unsigned int getSize();
  float getW(unsigned int index) const;

  Vec3f& getCenter(const unsigned int index) const;

  void setSize(const unsigned int _size);
  void setW(const unsigned int index, const float val);
  void setCenter(const unsigned int index, const Vec3f& _center);

  void setFilter(ConstraintFilter *f, int parm) {
    cf=f;
    filterParm = parm;
  }

  static const ConstraintFilter *NULL_FILTER;
  
 protected:
  unsigned int size;
  float *w;
  Vec3f *center;
  float *c;

  void applyFilter(ConstraintSet &cs);
  virtual int computeRBF(const ConstraintSet &cs) = 0;

  virtual double phi(const double r) const = 0;
  virtual double dist(const Vec3f &p1, const Vec3f &p2) const
    {
      return (p1 - p2).length();
    }

 private:
  ConstraintFilter *cf;
  int filterParm;
};

inline void
ConstructRBF::applyFilter(ConstraintSet &cs)
{
  cf->filter(cs, filterParm);
}

inline unsigned int
ConstructRBF::getSize()
{
  return size;
}

inline float
ConstructRBF::getW(unsigned int index) const
{
  return w[index];
}

inline Vec3f&
ConstructRBF::getCenter(const unsigned int index) const
{
  return center[index];
}

inline void
ConstructRBF::setSize(const unsigned int _size)
{
  size = _size;

  delete[] w;
  delete[] center;
  
  w = new float[size];
  center = new Vec3f[size];
}

inline void
ConstructRBF::setW(const unsigned int index, const float val)
{
  w[index] = val;
}

inline void
ConstructRBF::setCenter(const unsigned int index, const Vec3f& _center)
{
  center[index] = _center;
}

#endif /* CONSTRUCTRBF_H */
