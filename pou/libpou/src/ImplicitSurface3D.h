#ifndef IMPLICITESURFACE3D_H
#define IMPLICITESURFACE3D_H

#include "ConstructRBFPOU.h"
#include "Constraint.h"
#include "PointSet.h"
#include <string>
#include <vector>

class ImplicitSurface3D {
 public:
  ImplicitSurface3D(ConstructRBFPOU::TypeRBF _type = 
		    ConstructRBFPOU::TRIHARMONIC);
  ~ImplicitSurface3D();
  void compute(PointSet &ps);
  void compute(PointSet &ps, unsigned int size);

  void load(const std::string &filename);
  void save(const std::string &filename) const;

  float eval(const Vec3f &p) const { return rbf->eval(p); }
  void evalNormal(const Vec3f &p, Vec3f &v) const {
    return rbf->evalNormal(p, v);
  }
  void evalGradian(const Vec3f &p, Vec3f &v) const {
    return rbf->evalGradian(p, v);
  }
  const ConstructRBFPOU *getRBFPOU() const;
  void setProjDist(float d) {
    projDist=d;
  }
  float getProjDist() {
    return projDist;
  }

 private:
  ConstructRBFPOU *rbf;
  ConstructRBFPOU *r,*g,*b;
  ConstraintSet *cs;
  ConstraintFilter *rgb, *nz;
  float projDist;
};

inline const ConstructRBFPOU *ImplicitSurface3D::getRBFPOU() const {
  return rbf;
}

#endif /* IMPLICITESURFACE3D_H */
