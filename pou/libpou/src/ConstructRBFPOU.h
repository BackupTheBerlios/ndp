#ifndef CONSTRUCTRBFPOU_H
#define CONSTRUCTRBFPOU_H

#include <vector>
#include <iostream>
#include <fstream>

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
  void setFilter(ConstraintFilter *f, int p);

  void setCallBack(void (*c)(int, int), int s) {
    if (s!=0)
      step=s;
    callback=c;
  }

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
  int step;
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
