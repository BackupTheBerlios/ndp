#ifndef IMPLICITESURFACE3D_H
#define IMPLICITESURFACE3D_H

#include "ConstructRBFPOU.h"

class ImplicitSurface3D {
 public:
  ImplicitSurface3D(ConstructRBFPOU::TypeRBF _type = 
		    ConstructRBFPOU::TRIHARMONIC);
  ~ImplicitSurface3D();
  void load(std::string filename);
  void save(std::string filename) const;

  float eval(const Vec3f &p) const;
  void evalNormal(const Vec3f &p, Vec3f &v) const;
  void evalGradian(const Vec3f &p, Vec3f &v) const;
  const ConstructRBFPOU *getRBFPOU() const;

 private:
  ConstructRBFPOU *rbf;
};

inline const ConstructRBFPOU *ImplicitSurface3D::getRBFPOU() const {
  return rbf;
}

#endif /* IMPLICITESURFACE3D_H */
