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
  int compute(const std::vector<Constraint> &cs) ;
  float eval(const Vec3f &p) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  void jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const;
  float getL(const BoxVolume&) const;
  void load(std::ifstream &stream);
  void save(std::ofstream &stream) const;
 protected:
  double phi(const double r) const
    {
      return r*r*r;
    }
  
  double dist(const Vec3f& p1, const Vec3f& p2) const
    {
      return (p1 - p2).length();
    }
};

#endif /* CONSTRUCTRBFTRIHARMONIC_H */
