/**
 * @file   ConstructRBFPOU.h
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:43:35 2004
 * 
 * @brief  rbf reconstruction using an octree
 *
 * $Log: ConstructRBFPOU.h,v $
 * Revision 1.8  2004/04/07 08:03:17  leserpent
 * Added a	default value to the parameter p of setFilter
 *
 * Revision 1.7  2004/04/06 16:49:32  leserpent
 * ConstructRBFPOU::setcallback() can take two more parameters in order
 * to show only one progress dialog during color computing.
 *
 * Revision 1.6  2004/04/06 16:15:22  leserpent
 * Changed getThresholds() to getThreMin/ThreMax().
 *
 * Revision 1.5  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */

#ifndef CONSTRUCTRBFPOU_H
#define CONSTRUCTRBFPOU_H

#include <vector>
#include <iostream>
#include <fstream>
#include <utility>

#include "AreaSetOctree.h"
#include "math/vector3.h"
#include "box3d.h"
#include "ConstructRBF.h"
#include "ConstructRBFBiHarmonic.h"
#include "ConstructRBFTriHarmonic.h"
#include "ConstructRBFThinPlate.h"

class ConstructRBFPOU {
 public:

  enum TypeRBF{
    BIHARMONIC,
    TRIHARMONIC,
    THINPLATE
  };
  enum Flags {
    OK_FLAG
  };

  ConstructRBFPOU(TypeRBF _type = TRIHARMONIC);
  ~ConstructRBFPOU();
  void compute(ConstraintSet& cs, const AreaSet *octree = 0);
  void setThresholds(unsigned int tMin, unsigned tMax);
  float eval(const Vec3f &p) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  void evalNormal(const Vec3f &p, Vec3f &v) const;

  void load(std::ifstream &stream);
  void save(std::ofstream &stream) const;
  const AreaSet *getOctree();  
  void setFilter(ConstraintFilter *f, int p = 0);

  void setCallBack(void (*c)(int, int), int s, int pass = 0, int numPass = 1) {
    step = (s!=0)?s:step;
    this->pass = pass;
    this->numPass = (numPass!=0)?numPass:1;
    callback=c;
  }

  unsigned int getThreMin() { return threMin; }
  unsigned int getThreMax() { return threMax; }

 private:
  AreaSet* cells;
  std::vector<ConstructRBF*> rbf;
  std::vector<Flags> flag;
  TypeRBF type;
  unsigned int threMin, threMax;
  float overlap;
  ConstraintFilter *cf;
  int filterParm;

  ConstructRBF *newRBF(void);
  
  void applyFilter(ConstraintSet &cs);
  void (*callback)(int v, int max);
  int step, pass, numPass;
};

inline const AreaSet *
ConstructRBFPOU::getOctree()
{
  return cells;
}

inline void
ConstructRBFPOU::setFilter(ConstraintFilter *f, int p)
{
  cf=f;
  filterParm=p;
}

inline void 
ConstructRBFPOU::applyFilter(ConstraintSet &cs)
{
  cf->filter(cs, filterParm);
}

#endif /* CONSTRUCTRBFPOU_H */
