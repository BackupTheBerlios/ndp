/**
 * @file   AreaSphere.cc
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:53:34 2004
 * 
 * @brief  Area management using spheres
 *
 * $Log: AreaSphere.cc,v $
 * Revision 1.5  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
#include "AreaSphere.h"
#include "W.h"
#include "math/vector3.h"
#include "box3d.h"

static const float SPHERE_INTERSECT_EPSILON = 0.0001;

bool 
AreaSphere::intersect(const Vec3f& p) const
{
  return (p.dist2(center) < radius2);
}

bool 
AreaSphere::intersectRay (const Vec3f & origin, const Vec3f & direction,
			  float & depth1, float & depth2) const 
{
  float OCSquared, tClosestApproach, halfChord, tHalfChordSquared;
  Vec3f originToCenter;
  
  originToCenter = center - origin;
  
  OCSquared =  originToCenter.dot (originToCenter);
  
  tClosestApproach = originToCenter.dot (direction);

  if ((OCSquared >= radius2) && (tClosestApproach < SPHERE_INTERSECT_EPSILON))
    return false;
  
  tHalfChordSquared = radius2 - OCSquared +
    (tClosestApproach * tClosestApproach);

  if (tHalfChordSquared > SPHERE_INTERSECT_EPSILON)
  {
    halfChord = sqrt (tHalfChordSquared);
    depth1 = tClosestApproach - halfChord;
    depth2 = tClosestApproach + halfChord;
    
    return true;
  }

  return false;
}
 
float 
AreaSphere::w(const Vec3f& p) const
{
  if (! intersect(p))
    return 0;
  
  float dist = p.dist(center) / radius; 

  return w2(dist);
}

void AreaSphere::wd(const Vec3f& p, Vec3f& v) 
{
  if (! intersect(p)){
    v.setValues(0,0,0);  
    std::cout << " Bailed out of wd. " << std::endl;
    exit(1);
    return;
  }
  
  float distpur = p.dist(center);
  float dist = p.dist(center) / radius; 
  float wderived = w2d(dist);
  
  float d2r = 1/(radius * distpur);
  
  v.setValues( -wderived * d2r * (center.x - p.x),
               -wderived * d2r * (center.y - p.y),  
               -wderived * d2r * (center.z - p.z) );
   
}

Vec3f& 
AreaSphere::getCenter(void)
{
  return center;
}

void 
AreaSphere::save(std::ostream& stream)
{
  stream << center[0] << " " << center[1] << " " << center[2] << " "
	 << radius << std::endl;
}

Area**
AreaSphere::subdivide(void)
{
  Area** newArea = new Area*[8];
  float r = radius / (2*sqrt(3.0f));
  float r2 = radius / 2 ;
  
  newArea[0] = new AreaSphere(center + Vec3f(-r,-r,-r), r2);
  newArea[1] = new AreaSphere(center + Vec3f(-r,-r,r), r2);
  newArea[2] = new AreaSphere(center + Vec3f(-r,r,-r), r2);
  newArea[3] = new AreaSphere(center + Vec3f(-r,r,r), r2);
  
  newArea[4] = new AreaSphere(center + Vec3f(r,-r,-r), r2);
  newArea[5] = new AreaSphere(center + Vec3f(r,-r,r), r2);
  newArea[6] = new AreaSphere(center + Vec3f(r,r,-r), r2);
  newArea[7] = new AreaSphere(center + Vec3f(r,r,r), r2);

  return newArea;
}


void 
AreaSphere::grow(const float val)
{
  radius *= (1.0f + val);
  radius2 = radius*radius;
}

void 
AreaSphere::reduce(const float val)
{
  radius /= (1.0f + val);
  radius2 = radius*radius;
}
