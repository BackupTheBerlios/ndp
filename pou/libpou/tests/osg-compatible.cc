/*OpenSG accumule les imprecisions*/
/*OpenSG renvoit epsilon comme val max si tous le vecteur est entierement neg*/
/*operator/ n'existe pas*/
#include <limits>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#include <OpenSG/OSGVector.h>
#include <OpenSG/OSGBoxVolume.h>

#include "vector3.h"
#include "box3d.h"

#define NUMVECTOR 100000
#define RANGE 1000
#define EPSILON 1

/*------------------------*/

std::ostream& operator<<(std::ostream& os, const osg::BoxVolume& b) {
  return os << b.getMin() << " | " << b.getMax();
}

float frand() {
  return (((float)rand()/(float)RAND_MAX) - 0.5f)*2*RANGE;
}

template<typename Iterator, typename InputIterator, typename Function>
void for_each_binary(Iterator first, Iterator last,
                     InputIterator firstArg, Function f)
{
  while( first != last )
    f(*first++, *firstArg++);
}

/*------------------------*/

bool isequal(float a, float b) {
  return (((a-b)>-EPSILON)&&((a-b)<EPSILON));
}

bool isequalBool(bool a, bool b) {
  return a==b;
}

bool isequalVector(const Vec3f& a, const osg::Vec3f& b) {
  return isequal(a.x, b.x()) && isequal(a.y, b.y()) && isequal(a.z, b.z());
}

bool isequalVectorDumb(const Vec3f& a, const Vec3f& b) {
  return isequal(a.x, b.x) && isequal(a.y, b.y) && isequal(a.z, b.z);
}

/*-Box3d-----------------------*/
bool isequalPnt(const Vec3f& a, const osg::Pnt3f& b) {
  return isequalVector(a, osg::Vec3f(b));
}

bool isequalBox(const Box3f& a, const osg::BoxVolume& b) {
  return isequalVector(a.getMin(), osg::Vec3f(b.getMin())) &&
    isequalVector(a.getMax(), osg::Vec3f(b.getMax()));
}

/*--------------------------*/

template<typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename Contain, typename ContainOSG, typename Predicate>
void checkBinaryError(ContainSource &source, ContainSourceOSG &sourceOSG,
                      ContainSource2 &source2, ContainSource2OSG &source2OSG,
                      Contain res, ContainOSG resOSG, Predicate eq)
{
  using namespace std;
  pair<typename Contain::iterator, typename ContainOSG::iterator> p;
  p = mismatch(res.begin(), res.end(), resOSG.begin(), eq);
  if(p.first == res.end())
    cout << " OK" << endl;
  else {
    cout << "***Error***" << endl;
    int i = distance(res.begin(), p.first);
    cout <<"MATH: (" << source[i] << ") (" << source2[i] << ") = " << fixed << *p.first << resetiosflags(ios::floatfield)  << endl;
    cout << "OSG: (" << sourceOSG[i] << ") (" << source2OSG[i] << ") = " << fixed << *p.second << resetiosflags(ios::floatfield) << endl;
  }
}

template<typename ContainSource, typename ContainSourceOSG,
         typename Contain, typename ContainOSG, typename Predicate>
void checkUnaryError(ContainSource &source, ContainSourceOSG &sourceOSG,
                     Contain res, ContainOSG resOSG, Predicate eq)
{
  using namespace std;
  pair<typename Contain::iterator, typename ContainOSG::iterator> p;
  p = mismatch(res.begin(), res.end(), resOSG.begin(), eq);
  if(p.first == res.end())
    cout << " OK" << endl;
  else {
    cout << "***Error" << endl;
    int i = distance(res.begin(), p.first);
    cout <<"(" << source[i] << ") = " << fixed << *p.first << resetiosflags(ios::floatfield)  << endl;
    cout <<"(" << sourceOSG[i] << ") = " << fixed << *p.second << resetiosflags(ios::floatfield)  << endl;
  }
}

/*------------------------------*/

/*1 argument and non void return value*/
template<typename UnaryFunction1, typename UnaryFunction2,
         typename ContainSource1, typename ContainSource2,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testUnary(ContainSource1 &source, ContainSource2 &sourceOSG,
               ContainDest &dest, ContainDestOSG &destOSG,
               UnaryFunction1 f, UnaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  transform(source.begin(), source.end(), dest.begin(), f);
  transform(sourceOSG.begin(), sourceOSG.end(), destOSG.begin(), fOSG);
  checkUnaryError(source, sourceOSG, dest, destOSG, eq);
}

/*1 argument and void return value*/
template<typename UnaryFunction1, typename UnaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename BinaryPredicate>
void testApply(ContainSource &source, ContainSourceOSG &sourceOSG,
               UnaryFunction1 f, UnaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  ContainSource tmp(source.size());
  ContainSourceOSG tmpOSG(sourceOSG.size());
  pair<typename ContainSource::iterator, typename ContainSourceOSG::iterator> p;
  copy(source.begin(), source.end(), tmp.begin());
  copy(sourceOSG.begin(), sourceOSG.end(), tmpOSG.begin());
  for_each(tmp.begin(), tmp.end(), f);
  for_each(tmpOSG.begin(), tmpOSG.end(), fOSG);
  checkUnaryError(source, sourceOSG, tmp, tmpOSG, eq);
}

/*2 arguments and non void return value*/
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testBinary(ContainSource &source, ContainSourceOSG &sourceOSG,
                ContainSource2 &source2, ContainSource2OSG &source2OSG,
                ContainDest &dest, ContainDestOSG &destOSG,
                BinaryFunction1 f, BinaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  transform(source.begin(), source.end(), source2.begin(), dest.begin(), f);
  transform(sourceOSG.begin(), sourceOSG.end(), source2OSG.begin(), destOSG.begin(), fOSG);
  checkBinaryError(source, sourceOSG, source2, source2OSG, dest, destOSG, eq);
}

/*2 arguments and void return value*/
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainSource2, typename ContainSource2OSG,
         typename BinaryPredicate>
void testBinary(ContainSource &source, ContainSourceOSG &sourceOSG,
                ContainSource2 &source2, ContainSource2OSG &source2OSG,
                BinaryFunction1 f, BinaryFunction2 fOSG, BinaryPredicate eq)
{
  using namespace std;
  ContainSource tmp(source.size());
  ContainSourceOSG tmpOSG(sourceOSG.size());

  copy(source.begin(), source.end(), tmp.begin());
  copy(sourceOSG.begin(), sourceOSG.end(), tmpOSG.begin());
  for_each_binary(tmp.begin(), tmp.end(), source2.begin(), f);
  for_each_binary(tmpOSG.begin(), tmpOSG.end(), source2OSG.begin(), fOSG);
  checkBinaryError(source, sourceOSG, source2, source2OSG, tmp, tmpOSG, eq);
}

/*2 arguments, void return value and result into second argument */
template<typename BinaryFunction1, typename BinaryFunction2,
         typename ContainSource, typename ContainSourceOSG,
         typename ContainDest, typename ContainDestOSG,
         typename BinaryPredicate>
void testBinarySecond(ContainSource &source, ContainSourceOSG &sourceOSG,
                      ContainDest &dest, ContainDestOSG &destOSG,
                      BinaryFunction1 f, BinaryFunction2 fOSG,
                      BinaryPredicate eq)
{
  using namespace std;

  for_each_binary(source.begin(), source.end(), dest.begin(), f);
  for_each_binary(sourceOSG.begin(), sourceOSG.end(), destOSG.begin(), fOSG);
  checkUnaryError(source, sourceOSG, dest, destOSG, eq);
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

  return 0;
}
