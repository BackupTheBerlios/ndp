/**
 * @file   ConstructRBFBiHarmonic.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:44:31 2004
 * 
 * @brief  rbf reconstruction using biharmonics
 * 
 * $Log: ConstructRBFBiHarmonic.h,v $
 * Revision 1.6  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.5  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
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
  void evalGradient(const Vec3f &p, Vec3f &v) const;
  void jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const;
  float getL(const BoxVolume&) const;
  void load(std::istream &stream);
  void save(std::ostream &stream) const;

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
