/**
 * @file   AreaCube.h
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:48:35 2004
 * 
 * @brief  Area management using cubes
 * 
 * $Log: AreaCube.h,v $
 * Revision 1.5  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#ifndef AREACUBE_H
#define AREACUBE_H

#include <fstream>

#include "math/vector3.h"
#include "box3d.h"

#include "Area.h"


class AreaCube : public Area
{
private:
  BoxVolume box;
  Vec3f center;

public:
  AreaCube(void): box() {}

  AreaCube(const Vec3f& p1, const Vec3f& p2): box(p1, p2) {
    box.getCenter(center);
  }
  
  AreaCube(const float x0, const float y0, const float z0, 
	   const float x1, const float y1, const float z1): 
    box(x0, y0, z0, x1, y1, z1) {
    box.getCenter(center);
  }

  virtual ~AreaCube() {}

  virtual bool intersect(const Vec3f& p) const;
  virtual bool intersectRay (const Vec3f & origin, const Vec3f & direction,
			     float & depth1, float & depth2) const;
  virtual float w(const Vec3f& p) const;
  virtual void wd(const Vec3f& p, Vec3f &v);


  virtual Vec3f& getCenter(void);
  
  virtual Area** subdivide(void);
  virtual void grow(const float val);
  virtual void reduce(const float val) {}

  virtual void save(std::ostream& stream);

};

#endif
