#include <iostream>
#include <iomanip>
#include <cmath>

#include "math/matrix.h"
#include "math/quaternion.h"

using namespace std;

//v must be (1,0,0), (0,1,0) or (0,0,1)
void testUnitToMatrix(const Vec3f &v) {
  Matrix4f m;
  Quaternionf q; 
  float s, c;
  
  cout << endl << "Input quaternion: rotation of 2PI around (" << v << ")"
       << endl;
  s = sin(M_PI);
  c = cos(M_PI);
  q.setValues(v.x*s, v.y*s, v.z*s, c);
  cout << "Result: ";
  q.unitToMatrix44(m);
  if(m.isIdentity())
    cout << "Identity" <<  " OK!" << endl;
  else
    cout << "*** Error: Identity was expected intead of: "  << endl
         << m << endl;

  Matrix4f expected;
  expected.setValues(2*v.x-1,  0,  0, 0,
                     0, 2*v.y-1,  0, 0,
                     0,  0, 2*v.z-1, 0,
                     0,  0,  0, 1);
  cout << endl << "Input quaternion: rotation of PI around (" << v << ")"
       << endl;
  s = sin(M_PI/2);
  c = cos(M_PI/2);
  q.setValues(v.x*s, v.y*s, v.z*s, c);
  cout << "Result: ";
  q.unitToMatrix44(m);
  if(m == expected)
    cout << "expected rotation OK!" << endl;
  else {
    cout << "*** Error:" << endl << m << "Expected: " << endl;
    cout << expected;
  }
}

int main() {
  Matrix4f m;
  Quaternionf q, q2, s(4,4,4,4);
 
  cout << setprecision(1) << fixed;
  cout << "norm(" << q << ")" << " = " << q.norm() << endl;
  cout << "norm(" << s << ")" << " = " << s.norm() << endl;

  q.setValues(1,1,1,1);

  cout << endl << "---------------------" << endl;
  cout << "Testing Quaternionf::unitToMatrix44(Matrix4f &m)..." << endl;
  cout << "---------------------" << endl;
  cout << "Input quaternion: Identity" << endl;
  q.setValues(0,0,0,1);
  cout << "Result: " ;
  q.unitToMatrix44(m);
  if(m.isIdentity())
    cout << "Identity" <<  " OK!" << endl;
  else
    cout << "*** Error: Identity was expected intead of: "  << endl
         << m << endl;
  testUnitToMatrix(Vec3f(1, 0, 0));
  testUnitToMatrix(Vec3f(0, 1, 0));
  testUnitToMatrix(Vec3f(0, 0, 1));

  cout << endl << "---------------------" << endl;
  cout << "Testing Quaternionf::operator*(Quaternion &q)..." << endl;
  cout << "---------------------" << endl;
  cout << "Input quaternions: rotation of angle aplha and -alpha around an arbitrary axis" << endl;
  float u = sin(1.8);
  float v = cos(1.8);
  Vec3f axis(1.3, 3.5, 9.1);
  axis.normalize();
  q.setValues(axis.x*u, axis.y*u, axis.z*u, v);
  u = sin(-1.8);
  v = cos(-1.8);
  q2.setValues(axis.x*u, axis.y*u, axis.z*u, v);
  s = q*q2;
  cout << fixed << q << " * " << q2 << " = " << fixed << s << endl;
  cout << "Result: ";
  if( s.isIdentity() )
    cout << "Identity" <<  " OK!" << endl;
  else {
    cout << "*** Error:" << endl << s << endl;
    cout << "Expected: Identity" << endl;
  }    
  
  return 0;
}
