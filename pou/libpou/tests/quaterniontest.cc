#include <iostream>
#include <iomanip>
#include <cmath>

#include "math/matrix.h"
#include "math/quaternion.h"

using namespace std;

//v must be (1,0,0), (0,1,0) or (0,0,1)
void testUnitToMatrix(const Vec3f &v) {
  Matrix4f m;
  Quaternionf q, r;
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
  Quaternionf q, r, s(1,2,3,1), t;
 
  cout << setprecision(1) << fixed;
  cout << s << " + " << s << " = " << q+s << endl;
  cout << "norm(" << q << ")" << " = " << q.norm() << endl;
  cout << "norm(" << s << ")" << " = " << s.norm() << endl;

  r = s;
  r.normalize();
  t = r.conjugate();
  cout << "conjugate(normalize(" << s << ")*normalize(" << s << ") = " << t*r << endl;

  q.setValues(1,1,1,1);
  cout << q << " * " << 4 << " = " << q*4 << endl;
  cout << q << " + " << q << " = " << q+q << endl;
  cout << q << " / " << 2 << " = " << q/2 << endl;

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
  cout << "Input quaternions: rotation of PI and -PI around axis (1,0,0)" << endl;
  q.setValues(1*sin(M_PI/2),0,0,cos(M_PI/2));
  Quaternionf q2;
  q2.setValues(1*sin(-M_PI/2),0,0,cos(-M_PI/2));
  r = q*q2;
  cout << fixed << q << " * " << q2 << " = " << fixed << r << endl;
  cout << "Result: ";
  if( r.isIdentity() )
    cout << "Identity" <<  " OK!" << endl;
  else {
    cout << "*** Error:" << endl << r << endl;
    cout << "Expected: Identity" << endl;
  }    

  
  return 0;
}
