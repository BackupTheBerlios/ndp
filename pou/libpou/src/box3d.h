/**
 * @file   box3d.h
 * @author Benjamin Grange
 * @date   March 
 * 
 * @brief  Support for bounding boxes
 * 
 * $Log: box3d.h,v $
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 */


#ifndef BOX3D_H
#define BOX3D_H

#include <iostream>
#include <cmath>
#include "math/vector3.h"

template<class T>
class Box3D
{
private:
  static const T huge;
  Vector3<T> min, max;

public:
  Box3D():min(huge, huge, huge), max(-huge, -huge, -huge) {}
  Box3D(const Vector3<T>& inMin, const Vector3<T>& inMax):min(inMin), max(inMax) {}
  Box3D(T x0, T y0, T z0, T x1, T y1, T z1):min(x0, y0, z0), max(x1, y1, z1) {}
  
  Vector3<T> getMin() const { return min; }
  
  Vector3<T> getMax() const { return max; }
  
  Vector3<T> getCenter() const {
    return min + (max - min) * T(0.5);
  }

  void getCenter( Vector3<T>& v ) const {
    v = getCenter();
  }
  
  Vector3<T> getSize() const {
    return max - min;
  }
  
  void getSize( Vector3<T>& v ) const {
    v = getSize();
  }
  
  bool intersect(const Vector3<T>& v) const {
    return 
      v.x > min.x &&
      v.y > min.y &&
      v.z > min.z &&
      v.x < max.x &&
      v.y < max.y &&
      v.z < max.z;
  }
  
  void getBounds(Vector3<T>& u, Vector3<T>& v) const {
    u = min; v = max;
  }

  // -----------------
  
  void setBoundsByCenterAndSize(const Vector3<T>& center,
                                const Vector3<T>& size)
    {
      min.setValues(center.x - size.x/T(2.0), center.y - size.y/T(2.0),
                   center.z - size.z/T(2.0));
    
      max.setValues(center.x + size.x/T(2.0), center.y + size.y/T(2.0),
                   center.z + size.z/T(2.0));
    }
  
  void extendBy(const Vector3<T>& v) {
    min.setValues(std::min(min.x, v[0]), std::min(min.y, v[1]),
                 std::min(min.z, v[2]));
    max.setValues(std::max(max.x, v[0]), std::max(max.y, v[1]),
                 std::max(max.z, v[2]));
  }

  friend std::ostream& operator<<(std::ostream& os, const Box3D<T>& v) {
    return os << v.getMin() << " | " << v.getMax();
  }

};

template<class T>
const T Box3D<T>::huge = std::numeric_limits<T>::max();

typedef Box3D<float> Box3f;
typedef Box3D<float> BoxVolume;
typedef Box3D<double> Box3d;

#endif
