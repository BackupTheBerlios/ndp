#ifndef POINTSET_H
#define POINTSET_H

#include <list>
#include <vector>
#include "Area.h"
#include "math/vector3.h"
#include "box3d.h"

static const float colors[][3]={
  {0, 0, 0},
  {0.2, 0.2, 0.2},
  {0.4, 0.4, 0.4},
  {0.6, 0.6, 0.6},
  {0.8, 0.8, 0.8},
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1},
  {1, 1, 0},
  {0, 1, 1},
  {1, 0, 1}
};

class PointSet;

class Point
{
public:
  Vec3f pos;
  Vec3f norm;
  Vec3f rgb;

public:
  Point(const float x = 0, const float y = 0, const float z = 0,
	const float xn = 0, const float yn = 1, const float zn = 0,
	const float r = 0.5f, const float g = 0.5f, const float b = 0.5f)
    : pos(x, y, z), norm(xn, yn, zn), rgb(r, g, b)
  {
    norm.normalize();
  }

  Point(const Vec3f _pos, const Vec3f _norm, const Vec3f _rgb)
    :pos(_pos), norm(_norm), rgb(_rgb)
  {
    norm.normalize();
  }


  Point(const Point& p)
  {
    pos = p.pos;
    norm = p.norm;
    rgb = p.rgb;
  }
  
  ~Point()
  {
  }

  
  // ---.oOo.---

  void setPoint(float x0, float y0, float z0)
  {
    pos[0] = x0;
    pos[1] = y0;
    pos[2] = z0;
  }

  Vec3f& getPos(void)
  {
    return pos;
  }

  void setPos(const Vec3f& _pos)
  {
    pos = _pos;
  }


  Vec3f& getNorm(void)
  {
    return norm;
  }

  Vec3f& getRGB(void)
  {
    return rgb;
  }

  void getPos(float& x, float& y, float& z)
  {
    x = pos[0]; 
    y = pos[1]; 
    z = pos[2];
  }

  friend class PointSet;
};



typedef std::list<Point*> PointList;

class PointSet
{
private:
  PointList points;
  BoxVolume box;

  void clear();

public:
  PointSet():
    points(),box()
  {
  }

  //copy constructor
  PointSet(const PointSet& ps);

  //choose points from _ps_ included in  _a_
  PointSet(const PointSet& ps, const Area* a);

  //choose _number_ points from _ps_
  PointSet(const PointSet& ps, int number);

  ~PointSet() { clear(); }
  // ---.oOo.---
  
  void getIndicesInArea(const Area* a, 
			std::vector<unsigned int>& index,
			unsigned int nb);

  // ---.oOo.---

  PointList::const_iterator getBegin(void) const
  {
    return points.begin();
  }

  PointList::const_iterator getEnd(void) const
  {
    return points.end();
  }

  PointList::iterator getBegin(void)
  {
    return points.begin();
  }

  PointList::iterator getEnd(void)
  {
    return points.end();
  }

  // ---.oOo.---

  void add(Point* p)
  {
    points.push_back(p);
    box.extendBy(p->pos);
  }

  void load(const char* filename);
  void save(const char* filename);

  unsigned int size(void) const 
  {
    return points.size();
  }

  void removeAll(void)
  {
    clear();
  }

  void remove(PointList::iterator i)
  {
    points.erase(i);
  }

  void remove(Point* p)
  {
    points.remove(p);           // FIXME:Should we do a delete?
  }


  int getNearest(const Vec3f& p);
  int find(float x, float y);

  void randomSplit(const unsigned int number,
		   PointSet& ps1,
		   PointSet& ps2) const;
  
  
  const BoxVolume& getBoundingBox(void) const 
  {
    return box;
  }

  void display(void);

  Point * getNearestPoint (Point * p);
};
#endif

