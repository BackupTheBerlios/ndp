/**
 * @file   ConstructRBFBiHarmonic.h
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:44:31 2004
 * 
 * @brief  rbf reconstruction using biharmonics
 * 
 * $Log: ConstructRBFBiHarmonic.h,v $
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef CONSTRUCTRBFBIHARMONIC_H
#define CONSTRUCTRBFBIHARMONIC_H

#include <fstream>
#include "ConstructRBF.h"
#include "box3d.h"

class ConstructRBFBiHarmonic : public ConstructRBF {
 public:
  ConstructRBFBiHarmonic();
  ~ConstructRBFBiHarmonic();
  int computeRBF(const ConstraintSet &cs) ;
  float eval(const Vec3f &p) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  void jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const;
  float getL(const BoxVolume&) const;
  void load(std::ifstream &stream);
  void save(std::ofstream &stream) const;

 private:
  void setC(float cx,
            float cy,
            float cz,
            float cg);

  void getC(float& cx,
            float& cy,
            float& cz,
            float& cg);
 protected:
  virtual inline double phi(const double r) const
    {
      return r;
    }

};

#endif /* CONSTRUCTRBFBIHARMONIC_H */
