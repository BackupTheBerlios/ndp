/**
 * @file   AreaSphere.h
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:46:50 2004
 * 
 * @brief  Area management using a sphere
 * 
 * $Log: AreaSphere.h,v $
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef AREASPHERE_H
#define AREASPHERE_H

#include "math/vector3.h"
#include "box3d.h"
#include "Area.h"


class AreaSphere : public Area
{
private:
  Vec3f center;
  float radius;
  float radius2;

public:
  AreaSphere(void):
    center(0,0,0), radius(1)
  {
    radius2 = 1;
  }

  AreaSphere(const Vec3f& _center, float _radius):
    center(_center), radius(_radius)
  {
    radius2 = radius * radius;
  }

  AreaSphere(const AreaSphere& a):
    center(a.center), radius(a.radius)
  {
    radius2 = radius * radius;
  }

  AreaSphere(const BoxVolume& box, const float grow = 1.0f)
  {
    Vec3f boxSize;

    box.getCenter(center);
    box.getSize(boxSize);

    radius = boxSize.length() / 2 * grow;
    radius2 = radius * radius;
  }

  virtual ~AreaSphere()
  {}

  virtual bool intersect(const Vec3f& p) const;
  virtual bool intersectRay (const Vec3f & origin, const Vec3f & direction, float & depth1, float & depth2) const;
  virtual float w(const Vec3f& p) const;
  virtual void wd(const Vec3f& p, Vec3f &v);

  virtual Vec3f& getCenter(void);

  virtual void display(void);

  virtual void save(std::ostream& stream);

  virtual Area** subdivide(void);
  virtual void grow(const float val);
  virtual void reduce(const float val);

};

#endif
