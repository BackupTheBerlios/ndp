#ifndef CONSTRUCTRBFBIHARMONIC_H
#define CONSTRUCTRBFBIHARMONIC_H

#include "ConstructRBF.h"
#include "box3d.h"

class ConstructRBFBiHarmonic : public ConstructRBF {
 public:
  ConstructRBFBiHarmonic();
  ~ConstructRBFBiHarmonic();
  int compute(const std::vector<Constraint> &cs) ;
  float eval(const Vec3f &p) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  void jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const;
  float getL(const BoxVolume&) const;
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
