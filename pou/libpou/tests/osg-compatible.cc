#include <limits>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include <OpenSG/OSGVector.h>
#include <OpenSG/OSGBoxVolume.h>

#include "math/vector3.h"
#include "box3d.h"
#include "apply.h"
#include "equal.h"

#define NUMVECTOR 100000
#define RANGE 1000

std::ostream& operator<<(std::ostream& os, const osg::BoxVolume& b) {
  return os << b.getMin() << " | " << b.getMax();
}

float frand() {
  return (((float)rand()/(float)RAND_MAX) - 0.5f)*2*RANGE;
}

/*------------------------------*/

template<typename T>
void createVectorPool(std::vector<T> &dest, unsigned int numVector, int seed) {
  srand(seed);
  dest[0] = T(0,0,0);
  dest[0] = T(2*RANGE+2,2*RANGE+2,2*RANGE+2);
  for(int i = 1; i < numVector; i++) {
    float x = frand(); float y = frand(); float z = frand();
    dest[i] = T(x, y, z);
  }
}

template<typename T>
void createBoxPool(std::vector<T> &dest, unsigned int numBox, int seed) {
  srand(seed);
  dest[0] = T(-RANGE, -RANGE, -RANGE, RANGE, RANGE, RANGE);
  for(int i = 1; i < numBox; i++) {
    float x = frand(); float y = frand(); float z = frand();
    float size = std::abs(frand());
    dest[i] = T(x, y, z, x+size, y+size, z+size);
  }
}

void createScalarPool(std::vector<float> &dest, unsigned int numScalar,
                      int seed)
{
  srand(seed);
  dest[0] = 1;
  for(int i = 1; i < numScalar; i++) {
    float k = frand();
    dest[i] = (k == 0)?1:k;
  }
}

/*------------------------------*/

int
main(int argc, char** argv)
{
  using namespace std;
  vector<Box3f> boxPool(NUMVECTOR), boxRes(NUMVECTOR);
  vector<Vec3f> vectorPool(NUMVECTOR), vectorRes(NUMVECTOR);
  vector<Vec3f> vectorPool2(NUMVECTOR);
  vector<osg::Vec3f> vectorPoolOSG(NUMVECTOR), vectorPoolOSG2(NUMVECTOR);
  vector<osg::Vec3f> vectorResOSG(NUMVECTOR);
  vector<osg::Pnt3f> pntResOSG(NUMVECTOR), pntPoolOSG(NUMVECTOR);
  vector<osg::BoxVolume> boxPoolOSG(NUMVECTOR), boxResOSG(NUMVECTOR);
  vector<float> scalarPool(NUMVECTOR), scalarRes(NUMVECTOR);
  vector<float> scalarResOSG(NUMVECTOR);
  vector<bool> boolRes(NUMVECTOR), boolResOSG(NUMVECTOR);

  createVectorPool(vectorPool, NUMVECTOR, 1);
  createVectorPool(vectorPoolOSG, NUMVECTOR, 1);
  createVectorPool(vectorPool2, NUMVECTOR, 1);
  createVectorPool(vectorPoolOSG2, NUMVECTOR, 1);
  createVectorPool(pntPoolOSG, NUMVECTOR, 1);
  createScalarPool(scalarPool, NUMVECTOR, 10);
  createBoxPool(boxPool, NUMVECTOR, 1);
  createBoxPool(boxPoolOSG, NUMVECTOR, 1);

  /*Testing functions with 1 argument and non void return values*/
  cout << "Testing Vec3f::maxValue()...";
  testUnary(vectorPool, vectorPoolOSG, scalarRes, scalarResOSG, mem_fun_ref(&Vec3f::maxValue), mem_fun_ref(&osg::Vec3f::maxValue), isequal);

  cout << "Testing Vec3f::length()...";
  testUnary(vectorPool, vectorPoolOSG, scalarRes, scalarResOSG, mem_fun_ref(&Vec3f::length), mem_fun_ref(&osg::Vec3f::length), isequal);

  cout << "Testing Vec3f::operator-()(negate)...";
  testUnary(vectorPool, vectorPoolOSG, vectorRes, vectorResOSG, mem_fun_ref(static_cast<Vec3f (Vec3f::*)() const>(&Vec3f::operator-)), mem_fun_ref(static_cast<osg::Vec3f (osg::Vec3f::*)() const>(&osg::Vec3f::operator-)), isequalVector);

  /*Testing functions with 2 argument and non void return values*/
  cout << "Testing Vec3f::dist(const Vector3<T> &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, scalarRes, scalarResOSG, mem_fun_ref(&Vec3f::dist), mem_fun_ref(&osg::Vec3f::dist), isequal);

  cout << "Testing Vec3f::dist2(const Vector3<T> &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, scalarRes, scalarResOSG, mem_fun_ref(&Vec3f::dist2), mem_fun_ref(&osg::Vec3f::dist2), isequal);

  cout << "Testing Vec3f::operator*(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, scalarRes, scalarResOSG, mem_fun_ref(static_cast<float (Vec3f::*)(const Vec3f&) const>(&Vec3f::operator*)), mem_fun_ref(static_cast<float (osg::Vec3f::*)(const osg::Vec3f&) const>(&osg::Vec3f::operator*)), isequal);

  cout << "Testing Vec3f::dot(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, scalarRes, scalarResOSG, mem_fun_ref(&Vec3f::dot), mem_fun_ref(static_cast<float (osg::Vec3f::*)(const osg::Vec3f&) const>(&osg::Vec3f::dot)), isequal);

  cout << "Testing Vec3f::operator==(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, boolRes, boolResOSG, mem_fun_ref(&Vec3f::operator==), mem_fun_ref(&osg::Vec3f::operator==), isequalBool);

  cout << "Testing Vec3f::cross(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, vectorRes, vectorResOSG, mem_fun_ref(&Vec3f::cross), mem_fun_ref(&osg::Vec3f::cross), isequalVector);
  
  cout << "Testing Vec3f::operator-(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, vectorRes, vectorResOSG, mem_fun_ref(static_cast<Vec3f (Vec3f::*)(const Vec3f&) const>(&Vec3f::operator-)), mem_fun_ref(static_cast<osg::Vec3f (osg::Vec3f::*)(const osg::Vec3f&) const>(&osg::Vec3f::operator-)), isequalVector);

  cout << "Testing Vec3f::operator+(const Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, vectorRes, vectorResOSG, mem_fun_ref(&Vec3f::operator+), mem_fun_ref(&osg::Vec3f::operator+), isequalVector);

  cout << "Testing Vec3f::operator*(float k)...";
  testBinary(vectorPool, vectorPoolOSG, scalarPool, scalarPool, vectorRes, vectorResOSG, mem_fun_ref(static_cast<Vec3f (Vec3f::*)(float) const>(&Vec3f::operator*)), mem_fun_ref(static_cast<osg::Vec3f (osg::Vec3f::*)(float) const>(&osg::Vec3f::operator*)), isequalVector);

  /*Testing functions with 2 argumenst and void return value*/
  cout << "Testing Vec3f::operator+=(Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, mem_fun_ref(&Vec3f::operator+=), mem_fun_ref(&osg::Vec3f::operator+=), isequalVector);

  cout << "Testing Vec3f::operator-=(Vec3f &v)...";
  testBinary(vectorPool, vectorPoolOSG, vectorPool2, vectorPoolOSG2, mem_fun_ref(&Vec3f::operator-=), mem_fun_ref(&osg::Vec3f::operator-=), isequalVector);

  cout << "Testing Vec3f::operator*=(float k)...";
  testBinary(vectorPool, vectorPoolOSG, scalarPool, scalarPool, mem_fun_ref(&Vec3f::operator*=), mem_fun_ref(&osg::Vec3f::operator*=), isequalVector);

  cout << "Testing Vec3f::operator/=(float k)...";
  testBinary(vectorPool, vectorPoolOSG, scalarPool, scalarPool, mem_fun_ref(&Vec3f::operator/=), mem_fun_ref(&osg::Vec3f::operator/=), isequalVector);

  /*Testing functions 1 argument and void return value*/
  cout << "Testing Vec3f::normalize()...";
  testApply(vectorPool, vectorPoolOSG, mem_fun_ref(&Vec3f::normalize), mem_fun_ref(&osg::Vec3f::normalize), isequalVector);


  /********************Testing Box3d************************/
  cout << "\n\n----------Testing Box3d" << endl;

  
  /*Testing functions with 1 argument and non void return values*/
  cout << "Testing Box3f::getMin()...";
  testUnary(boxPool, boxPoolOSG, vectorRes, pntResOSG, mem_fun_ref(&Box3f::getMin), mem_fun_ref(&osg::BoxVolume::getMin), isequalPnt);

  cout << "Testing Box3f::getMax()...";
  testUnary(boxPool, boxPoolOSG, vectorRes, pntResOSG, mem_fun_ref(&Box3f::getMax), mem_fun_ref(&osg::BoxVolume::getMax), isequalPnt);

  /*functions with 2 arguments, void return value and result into second */
  /*argument */
  cout << "Testing Box3f::getCenter(Vec3f &v)...";
  testBinarySecond(boxPool, boxPoolOSG, vectorRes, pntResOSG, mem_fun_ref(static_cast<void (Box3f::*)(Vec3f&) const>(&Box3f::getCenter)), mem_fun_ref((&osg::BoxVolume::getCenter)), isequalPnt);

  cout << "Testing Box3f::getSize(Vec3f &v)...";
  testBinarySecond(boxPool, boxPoolOSG, vectorRes, vectorResOSG, mem_fun_ref(static_cast<void (Box3f::*)(Vec3f&) const>(&Box3f::getSize)), mem_fun_ref((&osg::BoxVolume::getSize)), isequalVector);

  /*Testing functions with 2 argumenst and non void return value*/
  cout << "Testing Box3f::intersect()...";
  testBinary(boxPool, boxPoolOSG, vectorPool, pntPoolOSG, boolRes, boolResOSG, mem_fun_ref(&Box3f::intersect), mem_fun_ref(static_cast<bool (osg::BoxVolume::*)(const osg::Pnt3f&) const>(&osg::BoxVolume::intersect)), isequalBool);

  cout << "Testing Box3f::extendBy(Vec3f &v)...";
  testBinary(boxPool, boxPoolOSG, vectorPool, pntPoolOSG, mem_fun_ref(&Box3f::extendBy), mem_fun_ref(static_cast<void (osg::BoxVolume::*)(const osg::Pnt3f&)>(&osg::BoxVolume::extendBy)), isequalBox);

  cout << "Testing Box3f::setBoundsByCenterAndSize(Vec3f &v, Vec3f &u)...";
  unsigned int i;
  for(i = 0; i < RANGE; i++) {
    Box3f b = boxPool[i];
    Vec3f v1 = vectorPool[i];
    Vec3f v2 = vectorPool2[i];
    b.setBoundsByCenterAndSize(v1, v2);

    osg::BoxVolume bOSG = boxPoolOSG[i];
    osg::Vec3f v1OSG = vectorPoolOSG[i];
    osg::Vec3f v2OSG = vectorPoolOSG2[i];    
    bOSG.setBoundsByCenterAndSize(v1OSG, v2OSG);
    
    if( !(isequalPnt(b.getMin(), bOSG.getMin()) &&
          isequalPnt(b.getMax(), bOSG.getMax()))) {
      cout <<"MATH: (" << v1 << ") (" << v2 << ") (" <<   boxPool[i] << ") = " << fixed << b << resetiosflags(ios::floatfield)  << endl;
      cout << "OSG: (" << v1OSG << ") (" << v2OSG << ") (" << boxPoolOSG[i] << ") = " << fixed << bOSG << resetiosflags(ios::floatfield) << endl;
      break;
    }
  }
  if( i == RANGE )
    cout << " OK" << endl;

  cout << "Testing Box3f::getBounds...";
  for(i = 0; i < RANGE; i++) {
    Vec3f v1, v2;
    boxPool[i].getBounds(v1, v2);

    osg::Vec3f v1OSG, v2OSG;
    boxPoolOSG[i].getBounds(v1OSG, v2OSG);
    
    if( !(isequalPnt(v1, v1OSG) &&
          isequalPnt(v2, v2OSG))) {
      cout <<"MATH: (" <<  boxPool[i] << ") = " << fixed << v1 << " "  << v2 << resetiosflags(ios::floatfield)  << endl;
      cout << "OSG: (" << boxPoolOSG[i] << ") = " << fixed << v1OSG << " " << v2OSG << resetiosflags(ios::floatfield) << endl;
      break;
    }
  }
  if( i == RANGE )
    cout << " OK" << endl;


  return 0;
}
