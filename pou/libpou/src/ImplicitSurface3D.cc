/**
 * @file   ImplicitSurface3D.cc
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:50:25 2004
 * 
 * @brief  Implicit surface support
 * 
 * $Log: ImplicitSurface3D.cc,v $
 * Revision 1.13  2004/04/07 08:00:18  leserpent
 * Use the	new constructor of ConstraintSet.
 * Added some const.
 *
 * Revision 1.12  2004/04/06 16:16:11  leserpent
 * Splitted compute() into computeRGB() and computeGeometry().
 * The Constraints Set is now a local variable in computeRGB/Geometry()
 * since it's not used anywhere else.
 * Constraints are now deleted after compute.
 *
 * Revision 1.11  2004/04/06 10:08:33  ob821
 * Removed comments
 *
 * Revision 1.10  2004/04/06 09:47:15  ob821
 * Changelog
 *
 * Revision 1.9  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#include "ImplicitSurface3D.h"
#include "PointSet.h"
#include "ConstraintSet.h"
#include "ConstraintFilter.h"
#include "helpers/deletor.h"

#include <fstream>


class ConstraintFilterRGB : public ConstraintFilter {
private:
  const PointSet *ps;
public:
  ConstraintFilterRGB(const PointSet *_ps) {
    ps=_ps;
  }
  void filter(ConstraintSet& cs, int parm) {
    PointList::const_iterator end=ps->getEnd();
    PointList::const_iterator i;
    int j;
    
    for( i=ps->getBegin(), j=0; i!=end; i++, j++) {
      Point *p=*i;
      cs[j]->setConstraint(p->getRGB()[parm]);
    }
  };
};

class ConstraintFilterNonZero : public ConstraintFilter {
private:
  const PointSet *ps;
  float projDist;
public:
  ConstraintFilterNonZero(const PointSet *_ps, float _projDist) {
    ps=_ps;
    projDist=_projDist;
  }
  void filter(ConstraintSet& cs, int parm) {
    std::cout << "filter nz" << std::endl;
    PointList::const_iterator end=ps->getEnd();
    PointList::const_iterator i;
    int j;
    
    for( i=ps->getBegin(), j=0; i!=end; i++, j++) {
      Point *p=*i;
      Vec3f center;
      Vec3f c=p->getNorm();
      cs[j]->setConstraint(0);
      if (c.isNull())
	continue;
      center= p->getPos() - (c*projDist);
      cs.add(new Constraint(center, 1));
      center= p->getPos() + (c*projDist);
      cs.add(new Constraint(center, -1));
    }
  };
};

ImplicitSurface3D::ImplicitSurface3D(ConstructRBFPOU::TypeRBF _type) {
  rbf = new ConstructRBFPOU(_type);
  r = new ConstructRBFPOU(_type);
  g = new ConstructRBFPOU(_type);
  b = new ConstructRBFPOU(_type);
  projDist=0.03f;
  rbf->setThresholds(rbf->getThreMin()*3, rbf->getThreMax()*3);
}

ImplicitSurface3D::~ImplicitSurface3D() {
  delete r;
  delete g;
  delete b;
  delete rbf;
}

void ImplicitSurface3D::computeRGB(const PointSet &ps) {
  const AreaSet *o;
  ConstraintSet cs(ps);
  ConstraintFilterRGB rgb(&ps);
  
  r->setFilter(&rgb, 0);
  r->compute(cs);
  o=r->getOctree();
  g->setFilter(&rgb, 1);
  g->compute(cs, o);
  b->setFilter(&rgb, 2);
  b->compute(cs, o);
  
  cs.removeDeleteAll(); // We no longer need our constraints
}

void ImplicitSurface3D::computeGeometry(const PointSet &ps) {
  ConstraintSet cs(ps);
  ConstraintFilterNonZero nz(&ps, projDist);

  rbf->setFilter(&nz);
  rbf->compute(cs);

  cs.removeDeleteAll();
}

void ImplicitSurface3D::compute(const PointSet &ps) {
  computeRGB(ps);
  computeGeometry(ps);
}

void ImplicitSurface3D::compute(const PointSet &ps, unsigned int size) {
  PointSet psFiltered(ps, size);
  compute(psFiltered);
}

void ImplicitSurface3D::computeRGB(const PointSet &ps, unsigned int size) {
  PointSet psFiltered(ps, size);
  computeRGB(psFiltered);
}

void ImplicitSurface3D::computeGeometry(const PointSet &ps, unsigned int size) {
  PointSet psFiltered(ps, size);
  computeGeometry(psFiltered);
}

void ImplicitSurface3D::load(const std::string &filename) {
  std::ifstream stream(filename.c_str());
  rbf->load(stream);
}

void ImplicitSurface3D::save(const std::string &filename) const {
  std::ofstream stream(filename.c_str());
  rbf->save(stream);
}
