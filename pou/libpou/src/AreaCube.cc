#include <GL/gl.h>
#include "AreaCube.h"
#include "W.h"
#include "vector3.h"
#include "box3d.h"

using namespace std;

bool 
AreaCube::intersect(const Vec3f& p) const
{
  return box.intersect(p);
}

// **********
// TO CHANGE
// **********

bool 
AreaCube::intersectRay (const Vec3f & origin, const Vec3f & direction, float & depth1, float & depth2) const 
{
  depth1 = 0.0;
  depth2 = 0.0;
  
  return false;
}

float 
AreaCube::w(const Vec3f& p) const
{
  if (! intersect(p))
      return 0;

  
  Vec3f b0 = box.getMin();
  Vec3f b1 = box.getMax();

  Vec3f d0 = p - b0;
  Vec3f d1 = b1 - p;

  Vec3f d = b1 - b0;

  float dist = (d0[0]*d1[0]*d0[1]*d1[1]*d0[2]*d1[2]) / 
    (d[0]*d[0]*d[1]*d[1]*d[2]*d[2]);

  dist *= 64;

  return w1(1-dist);
}

void AreaCube::wd(const Vec3f& p, Vec3f& v) 
{
  // TODO
   
}

Vec3f& 
AreaCube::getCenter(void)
{
  return center;
}
  

Area** 
AreaCube::subdivide(void)
{
  Vec3f b0 = box.getMin();
  Vec3f b1 = box.getMax();

  Vec3f bc(center);

  Vec3f corr = (b1-b0)*0.5*0.05    ;

  Area** newArea = new (Area*)[8];
   
  newArea[0] = new AreaCube(b0[0]-corr[0], b0[1]-corr[1], b0[2]-corr[2], 
			    bc[0]+corr[0], bc[1]+corr[1], bc[2]+corr[2]);
  newArea[1] = new AreaCube(b0[0]-corr[0], b0[1]-corr[1], bc[2]-corr[2], 
			    bc[0]+corr[0], bc[1]+corr[1], b1[2]+corr[2]);
  newArea[2] = new AreaCube(b0[0]-corr[0], bc[1]-corr[1], b0[2]-corr[2], 
			    bc[0]+corr[0], b1[1]+corr[1], bc[2]+corr[2]);
  newArea[3] = new AreaCube(b0[0]-corr[0], bc[1]-corr[1], bc[2]-corr[2], 
			    bc[0]+corr[0], b1[1]+corr[1], b1[2]+corr[2]);
  
  newArea[4] = new AreaCube(bc[0]-corr[0], b0[1]-corr[1], b0[2]-corr[2], 
			    b1[0]+corr[0], bc[1]+corr[1], bc[2]+corr[2]);
  newArea[5] = new AreaCube(bc[0]-corr[0], b0[1]-corr[1], bc[2]-corr[2], 
			    b1[0]+corr[0], bc[1]+corr[1], b1[2]+corr[2]);
  newArea[6] = new AreaCube(bc[0]-corr[0], bc[1]-corr[1], b0[2]-corr[2], 
			    b1[0]+corr[0], b1[1]+corr[1], bc[2]+corr[2]);
  newArea[7] = new AreaCube(bc[0]-corr[0], bc[1]-corr[1], bc[2]-corr[2], 
			    b1[0]+corr[0], b1[1]+corr[1], b1[2]+corr[2]);

  //newArea[4] = new AreaCube(bc[0], b0[1], b0[2], b1[0], bc[1], bc[2]);
  //newArea[5] = new AreaCube(bc[0], b0[1], bc[2], b1[0], bc[1], b1[2]);
  //newArea[6] = new AreaCube(bc[0], bc[1], b0[2], b1[0], b1[1], bc[2]);
  //newArea[7] = new AreaCube(bc[0], bc[1], bc[2], b1[0], b1[1], b1[2]);

  return newArea;
}
 
void 
AreaCube::grow(const float val)
{
  Vec3f size, sizeGrow;
  box.getSize(size);

  sizeGrow = size * (1.0f + val);

  box.setBoundsByCenterAndSize(center, sizeGrow);
}

void 
AreaCube::display(void)
{
#if 0
  Vec3f b0 = box.getMin();
  Vec3f b1 = box.getMax();

  glColor3f(1.0f, 1.0f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_QUADS);
  //bottom face
  glVertex3f(b0[0], b0[1], b0[2]);
  glVertex3f(b1[0], b0[1], b0[2]);
  glVertex3f(b1[0], b0[1], b1[2]);
  glVertex3f(b0[0], b0[1], b1[2]);
  //top face
  glVertex3f(b0[0], b1[1], b0[2]);
  glVertex3f(b1[0], b1[1], b0[2]);
  glVertex3f(b1[0], b1[1], b1[2]);
  glVertex3f(b0[0], b1[1], b1[2]);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  

  glBegin(GL_LINES);
  glVertex3f(b0[0], b0[1], b0[2]);
  glVertex3f(b0[0], b1[1], b0[2]);

  glVertex3f(b1[0], b0[1], b0[2]);
  glVertex3f(b1[0], b1[1], b0[2]);

  glVertex3f(b1[0], b0[1], b1[2]);
  glVertex3f(b1[0], b1[1], b1[2]);

  glVertex3f(b0[0], b0[1], b1[2]);
  glVertex3f(b0[0], b1[1], b1[2]);
  glEnd();
#endif
}


void 
AreaCube::save(ostream& stream)
{
  Vec3f b0p = box.getMin();
  Vec3f b1p = box.getMax();
  
  stream << b0p[0] << " "
	 << b0p[1] << " "
	 << b0p[2] << " "
	 << "-1.0" << " "
	 << b1p[0] << " "
	 << b1p[1] << " "
	 << b1p[2] << endl;
}





//test
/*
#include<iostream>
int 
main(void)
{
  AreaCube a(-1,-2,-3,1,2,3);
  

  cout << a.w(Vec3f(1,1,1)) << endl;
  cout << a.w(Vec3f(0.5,1,-1.5)) << endl;
  cout << a.w(Vec3f(0,0,0)) << endl;
  cout << a.w(Vec3f(0,-1.5,0)) << endl;  
  cout << a.w(Vec3f(1,2,3)) << endl;

  return 0;
}
*/
