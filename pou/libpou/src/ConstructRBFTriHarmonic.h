/**
 * @file   ConstructRBFTriHarmonic.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:41:29 2004
 * 
 * @brief  rbf reconstruction using triharmonics
 *
 * $Log: ConstructRBFTriHarmonic.h,v $
 * Revision 1.5  2004/04/20 11:16:39  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef CONSTRUCTRBFTRIHARMONIC_H
#define CONSTRUCTRBFTRIHARMONIC_H

#include <vector>
#include <fstream>

#include "ConstructRBF.h"
#include "box3d.h"

class ConstructRBFTriHarmonic : public ConstructRBF {
 public:
  ConstructRBFTriHarmonic();
  ~ConstructRBFTriHarmonic();
  float eval(const Vec3f &p) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  void jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const;
  float getL(const BoxVolume&) const;
  void load(std::istream &stream);
  void save(std::ostream &stream) const;
 protected:
  double phi(const double r) const
    {
      return r*r*r;
    }
  
  double dist(const Vec3f& p1, const Vec3f& p2) const
    {
      return (p1 - p2).length();
    }
 private:
  int computeRBF(const ConstraintSet &cs) ;

};

#endif /* CONSTRUCTRBFTRIHARMONIC_H */
