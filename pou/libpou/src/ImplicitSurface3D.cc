#include "ImplicitSurface3D.h"
#include "PointSet.h"
#include "ConstraintSet.h"
#include <fstream>
#include "ConstraintFilter.h"


class ConstraintFilterRGB : public ConstraintFilter {
private:
  PointSet *ps;
public:
  ConstraintFilterRGB(PointSet *_ps) {
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
  PointSet *ps;
  float projDist;
public:
  ConstraintFilterNonZero(PointSet *_ps, float _projDist) {
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
  cs = new ConstraintSet();
  projDist=0.03f;
}

ImplicitSurface3D::~ImplicitSurface3D() {
  delete r;
  delete g;
  delete b;
  delete rbf;
}

void ImplicitSurface3D::compute(PointSet &ps) {
  PointList::const_iterator end=ps.getEnd();
  PointList::const_iterator i;
  
  rgb = new ConstraintFilterRGB(&ps);
  nz = new ConstraintFilterNonZero(&ps, projDist);
  
  for(i=ps.getBegin(); i!=end; i++) {
    Point *p=*i;
    cs->add(new Constraint(p->getPos(), p->getRGB()[0]));
  } 
  r->setFilter(const_cast<ConstraintFilter *>(ConstructRBF::NULL_FILTER), 0);
  r->compute(*cs);
  g->setFilter(rgb, 1);
  g->compute(*cs);
  b->setFilter(rgb, 2);
  b->compute(*cs);
  rbf->setFilter(nz, 0);
  rbf->compute(*cs);
}

void ImplicitSurface3D::compute(PointSet &ps, unsigned int size) {
  PointSet psFiltered(ps, size);
  compute(psFiltered);
}

void ImplicitSurface3D::load(const std::string &filename) {
  std::ifstream stream(filename.c_str());
  rbf->load(stream);
}

void ImplicitSurface3D::save(const std::string &filename) const {
  std::ofstream stream(filename.c_str());
  rbf->save(stream);
}
