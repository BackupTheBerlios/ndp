#ifndef EQUAL_H
#define EQUAL_H

#define EPSILON 0.001 

bool isequal(float a, float b) {
  return (((a-b)>-EPSILON)&&((a-b)<EPSILON));
}

bool isequalBool(bool a, bool b) {
  return a==b;
}

bool isequalVector(const Vec3f& a, const osg::Vec3f& b) {
  return isequal(a.x, b.x()) && isequal(a.y, b.y()) && isequal(a.z, b.z());
}

/*-Box3d-----------------------*/
bool isequalPnt(const Vec3f& a, const osg::Pnt3f& b) {
  return isequalVector(a, osg::Vec3f(b));
}

bool isequalBox(const Box3f& a, const osg::BoxVolume& b) {
  return isequalVector(a.getMin(), osg::Vec3f(b.getMin())) &&
    isequalVector(a.getMax(), osg::Vec3f(b.getMax()));
}


#endif
