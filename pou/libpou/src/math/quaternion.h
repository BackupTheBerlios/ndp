#ifndef QUATERION_H
#define QUATERION_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>

#include "vector3.h"

template<class T>
class Quaternion {
public:
  Vector3<T> v;
  T w;

  Quaternion():v(0,0,0),w(1) {}
  Quaternion(const Vector3<T> &v, T w): v(v), w(w) {}
  Quaternion(T x, T y, T z, T w): v(x, y, z), w(w) {}

  void setValues(T x, T y, T z, T w) {
    v.setValues(x, y, z);
    this->w = w;
  }
  
  Quaternion<T> operator*(const Quaternion<T> &r) const {
    Quaternion<T> res;
//       res.v = v^r.v+r.w*v+w*r.v;
//       res.w = w*r.w-v*r.v;
//       std::cout << "Resulta1:" << res.v<<std::endl;
//      std::cout << "Resulta1:" << res.w<<std::endl;

    res.w = w*r.w - v.x*r.v.x - v.y*r.v.y - v.z*r.v.z;
    res.v.x = w*r.v.x + v.x*r.w + v.y*r.v.z - v.z*r.v.y;
    res.v.y = w*r.v.y + v.y*r.w + v.z*r.v.x - v.x*r.v.z;
    res.v.z = w*r.v.z + v.z*r.w + v.x*r.v.y - v.y*r.v.x ;

    //    std::cout << "Resultat2:" << res.w<<std::endl;
//      std::cout << "Resultat2:" << res.v<<std::endl;
            
    return res;
  }

  Quaternion<T> operator*(T k) const {
    return Quaternion<T>(v*k, k*w);
  }

  Quaternion<T> operator/(T k) const {
    assert(k!=0);
    return Quaternion<T>(v/k, w/k);
  }

  Quaternion<T> operator+(const Quaternion<T> &r) const {
    return Quaternion<T>(v+r.v, w+r.w);
  }

  Quaternion<T> conjugate() const {
    return Quaternion<T>(-v, w);
  }

  T norm() const {
    return std::sqrt(norm2());
  }

  T norm2() const {
    return v.length2()+w*w;
  }

  void normalize() {
    T k = T(1)/norm();
    v.x *= k; v.y *= k; v.z *= k;
    w *= k;
  }

  Quaternion<T> inverse() const {
    return conjugate()/norm();
  }

  void toIdentity() {
    setValues(0, 0, 0, 1);
  }
  
  void unitToMatrix44(T m[4][4]) const {
    T x2=v.x*v.x;
    T y2=v.y*v.y;
    T z2=v.z*v.z;
  
    m[0][0] = 1 - 2*y2 - 2*z2;
    m[1][0] = 2*v.x*v.y - 2*w*v.z;
    m[2][0] = 2*v.x*v.z + 2*w*v.y;
    m[0][1] = 2*v.x*v.y + 2*w*v.z;
    m[1][1] = 1 - 2*x2 - 2*z2;
    m[2][1] = 2*v.y*v.z - 2*w*v.x;
    m[0][2] = 2*v.x*v.z - 2*w*v.y;
    m[1][2] = 2*v.y*v.z + 2*w*v.x;
    m[2][2] = 1 - 2*x2 - 2*y2;
    m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0;
    m[3][3] = 1;  
  }

  // Reference: Stan Melax, Game Programming Gems
  void toRotationArc(Vector3<T> &u, Vector3<T> &v) {
    u.normalize();
    v.normalize();
    Vector3<T> w = u.cross(v);
    
    if(w.length2()<=Vec3f::epsilon)
      setValues(0, 0, 0, 1);
    else {
      T d = u*v;
      T s = std::sqrt((1+d)*2);
      setValues(w.x/s, w.y/s, w.z/s, s/T(2));
    }
  }

  void fromAxisRotation(T theta, T ax, T ay, T az) {
    T s = std::sin(theta/T(2));
    setValues(ax*s, ay*s, az*s, std::cos(theta/T(2)));
  }

  friend std::ostream& operator<<(std::ostream& os, const Quaternion<T>& q) {
    return os << "(" << q.v << "), " << q.w;
  }
};

typedef Quaternion<float> Quaternionf;

#endif
