/**
 * @file   Area.h
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:49:01 2004
 * 
 * @brief  One Area
 *
 * $Log: Area.h,v $
 * Revision 1.5  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
#ifndef AREA_H
#define AREA_H

#include <iostream>
#include <fstream>

#include "math/vector3.h"
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

    virtual void save(std::ostream& stream) = 0;
};

#endif /* AREA_H */
