/**
 * @file   ConstructRBFPOU.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:43:35 2004
 * 
 * @brief  rbf reconstruction using an octree
 *
 * $Log: ConstructRBFPOU.h,v $
 * Revision 1.14  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.13  2004/04/27 08:57:18  leserpent
 * abort ok
 *
 * Revision 1.12  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.11  2004/04/26 07:45:34  leserpent
 * Added a typedef for the callback function
 *
 * Revision 1.10  2004/04/25 12:09:24  pumpkins
 * error throw std::runtime_exception
 * Compute(POU) throw logic_error if (cs.size < threMin)
 *
 * Revision 1.9  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
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
#include <stdexcept>
#include <exception>


#include "AreaSetOctree.h"
#include "math/vector3.h"
#include "box3d.h"
#include "ConstructRBF.h"
#include "ConstructRBFBiHarmonic.h"
#include "ConstructRBFTriHarmonic.h"
#include "ConstructRBFThinPlate.h"
#include "helpers/callback.h"

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
  bool compute(ConstraintSet& cs, const AreaSet *octree = 0) 
    throw (std::logic_error);
  void setThresholds(unsigned int tMin, unsigned tMax);
  float eval(const Vec3f &p) const;
  void evalGradient(const Vec3f &p, Vec3f &v) const;
  void evalNormal(const Vec3f &p, Vec3f &v) const;

  void load(std::istream &stream);
  void save(std::ostream &stream) const;
  const AreaSet *getOctree() {
      return cells;
  }

  void setFilter(ConstraintFilter *f, int p = 0) {
    cf=f;
    filterParm=p;
  }

  void setCallBack(Callback c, int s, int pass = 0, int numPass = 1) {
    step = (s!=0)?s:step;
    this->pass = pass;
    this->numPass = (numPass!=0)?numPass:1;
    callback=c;
  }

  unsigned int getThreMin() {
    return threMin;
}
  unsigned int getThreMax() {
    return threMax;
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
  
  void applyFilter(ConstraintSet &cs) {
    cf->filter(cs, filterParm);
  }
  Callback callback;
  int step, pass, numPass;
};

#endif /* CONSTRUCTRBFPOU_H */
