/**
 * @file   ImplicitSurface3D.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:40:38 2004
 * 
 * @brief Support for implicit surface
 * 
 * $Log: ImplicitSurface3D.h,v $
 * Revision 1.12  2004/04/23 20:18:07  pumpkins
 * getOctree
 *
 * Revision 1.11  2004/04/20 11:16:39  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.10  2004/04/07 08:00:18  leserpent
 * Use the	new constructor of ConstraintSet.
 * Added some const.
 *
 * Revision 1.9  2004/04/06 16:49:32  leserpent
 * ConstructRBFPOU::setcallback() can take two more parameters in order
 * to show only one progress dialog during color computing.
 *
 * Revision 1.8  2004/04/06 16:16:11  leserpent
 * Splitted compute() into computeRGB() and computeGeometry().
 * The Constraints Set is now a local variable in computeRGB/Geometry()
 * since it's not used anywhere else.
 * Constraints are now deleted after compute.
 *
 * Revision 1.7  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef IMPLICITESURFACE3D_H
#define IMPLICITESURFACE3D_H

#include "ConstructRBFPOU.h"
#include "Constraint.h"
#include "PointSet.h"
#include <string>
#include <vector>
#include <cassert>

class ImplicitSurface3D {
 public:
  ImplicitSurface3D(ConstructRBFPOU::TypeRBF _type = 
		    ConstructRBFPOU::TRIHARMONIC);
  ~ImplicitSurface3D();
  void compute(const PointSet &ps);
  void compute(const PointSet &ps, unsigned int size);
  void computeRGB(const PointSet &ps, unsigned int size);
  void computeRGB(const PointSet &ps);
  void computeGeometry(const PointSet &ps, unsigned int size);
  void computeGeometry(const PointSet &ps);

  void load(const std::string &filename);
  void save(const std::string &filename) const;

  float eval(const Vec3f &p) const { return rbf->eval(p); }

  void evalNormal(const Vec3f &p, Vec3f &v) const {
    return rbf->evalNormal(p, v);
  }

  void evalGradian(const Vec3f &p, Vec3f &v) const {
    return rbf->evalGradian(p, v);
  }

  void evalColorRGB(const Vec3f &p, Vec3f &v) const {
    v.setValues(r->eval(p), g->eval(p), b->eval(p));
  }

  void setCallBack(void (*c)(int, int), int s) {
    assert(rbf&&r&&g&&b);
    rbf->setCallBack(c, s);
    r->setCallBack(c, s, 0, 3);
    g->setCallBack(c, s, 1, 3);
    b->setCallBack(c, s, 2, 3);
  }

  const ConstructRBFPOU *getRBFPOU() const;

  void setProjDist(float d) {
    projDist=d;
  }

  float getProjDist() {
    return projDist;
  }

  void setThresholds(unsigned int tMin, unsigned tMax) {
    assert(rbf&&r&&g&&b);
    rbf->setThresholds(tMin*3, tMax*3);
    r->setThresholds(tMin, tMax);
    g->setThresholds(tMin, tMax);
    b->setThresholds(tMin, tMax);
  }

  const AreaSet *getOctree() {
  	return rbf->getOctree();
  }

 private:
  ConstructRBFPOU *rbf;
  ConstructRBFPOU *r,*g,*b;
  float projDist;
};

inline const ConstructRBFPOU *ImplicitSurface3D::getRBFPOU() const {
  return rbf;
}

#endif /* IMPLICITESURFACE3D_H */
