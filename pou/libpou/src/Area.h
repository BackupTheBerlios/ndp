#ifndef AREA_H
#define AREA_H

#include <iostream>
#include <fstream>

#include "vector3.h"
#include "box3d.h"

class Area
{
 private:
 public:
    Area(void) {}

    virtual ~Area() {}

    virtual bool intersect(const Vec3f& p) const = 0;
    virtual bool intersectRay (const Vec3f& origin, const Vec3f& direction,
			       float& depth1, float& depth2) const = 0;
    virtual float w(const Vec3f& p) const = 0;
    virtual void wd(const Vec3f& p, Vec3f& v) = 0;

    virtual Vec3f& getCenter(void) = 0;
  
    virtual Area** subdivide(void) = 0;
    virtual void grow(const float val) = 0;
    virtual void reduce(const float val) = 0;

    virtual void display(void) = 0;
    virtual void save(std::ostream& stream) = 0;
};

#endif /* AREA_H */
