/**
 * @file   PointSet.cc
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:50:03 2004
 * 
 * @brief  PointSet management
 * 
 * $Log: PointSet.cc,v $
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#include <fstream>
#include <iostream>
#include "PointSet.h"
#include "math/vector3.h"
#include "box3d.h"

using namespace std;

PointSet::PointSet(const PointSet& ps):
  points(ps.points), box(ps.box)
{
}


PointSet::PointSet(const PointSet& ps, const Area* a):
  points()
{
  PointList::const_iterator i;
  for(i=ps.points.begin(); i!=ps.points.end(); i++)
    {
      Point* p = *i;
      if (a->intersect(p->getPos()))
	{
	  add(p);
	}
    }
}

PointSet::PointSet(const PointSet& ps, int number):
  points()
{
  unsigned int thre1 = number;
  unsigned int thre2 = ps.size();

  for(PointList::const_iterator i=ps.points.begin(); i!=ps.points.end(); i++)
    {
      float thre = (float)rand() / RAND_MAX;
      if (thre*thre2 < thre1)
	{
	  add(*i);
	  thre1 --;
	  thre2 --;
	}
      else
	{
	  thre2 --;
	}
    }
}

void
PointSet::clear()
{
  PointList::iterator end = getEnd();
  for(PointList::iterator i = getBegin(); i != end; ++i)
    delete *i;
  points.clear();
}

void 
PointSet::getIndicesInArea(const Area* a, 
			   vector<unsigned int>& index,
			   unsigned int nb)
{
  /*
  unsigned int size = points.size();
  unsigned int thre1 = nb;
  unsigned int thre2 = size;

  for(unsigned int i = 0; i<size; i++)
    {
      if (!a->intersect(points[i].getPos()))
	continue;
      
      float thre = (float)rand() / RAND_MAX;
      if (thre*thre2 < thre1)
	{
	  index.push_back(i);
	  thre1 --;
	  thre2 --;
	}
      else
	{
	  thre2 --;
	}
    }
  */
}

void 
PointSet::randomSplit(const unsigned int number,
		      PointSet& ps1,
		      PointSet& ps2) const
{
  unsigned int _size = size();
  unsigned int thre1 = number;
  unsigned int thre2 = _size;

  cout << "Splitting... " << flush;

  for(PointList::const_iterator i=points.begin(); i!=points.end(); i++)
    {
      float thre = (float)rand() / RAND_MAX;
      Point* p = *i;
      if (thre*thre2 < thre1)
	{
	  ps1.add(p);
	  thre1 --;
	  thre2 --;
	}
      else
	{
	  ps2.add(p);
	  thre2 --;
	}
    }

  cout << "OK. ( " 
       << ps1.size() << "+" 
       << ps2.size() << ")" << endl;
}



void 
PointSet::load(const char* filename)
{
  ifstream fs(filename);
  int counter = 0;

  clear();
  cout << "Reading... " << endl;
  while(!fs.eof())
    {
      float x, y, z, xn, yn, zn, r, g, b;
      
      fs >> x >> y >> z;
      fs >> xn >> yn >> zn;
      fs >> r >> g >> b;



      if (!fs.eof())
	{
	  //cout << counter << endl;
	  //cout << x << " " << y << " " << z << endl;
	  Point* p = new Point(x, y, z, xn, yn, zn, r, g, b);
	  add(p);
	  counter++;
	} 


      
      if ((counter & 0xfff) == 0xfff)
        cout << "\r " << counter << " read..." << flush;
      
      
      
    }
  
  fs.close();
  
  cout << "\r" << counter << " read...    " << endl;
  cout << "Bounding Box " << box.getMin() << " -- " << box.getMax() << endl;
}


void 
PointSet::save(const char* filename)
{
  ofstream fs(filename);
  
  cout << "Save " << filename << endl;
  for(PointList::iterator i=points.begin(); i!=points.end(); i++)
    {
      Point* p = *i;
      
      fs << p->pos[0] << " "
	 << p->pos[1] << " "
	 << p->pos[2] << endl;

      fs << p->norm[0] << " "
	 << p->norm[1] << " "
	 << p->norm[2] << endl;

      fs << p->rgb[0] << " "
	 << p->rgb[1] << " "
	 << p->rgb[2] << endl << endl;


    }
  
  fs.close();
}

int 
PointSet::find(float x, float y)
{
  for(PointList::iterator i=points.begin(); i!=points.end(); i++)
    {
      /*
      if (points[i].pos[0] == x && points[i].pos[1] == y)
	return i;
      */
    }
  return -1; 
}



int
PointSet::getNearest(const Vec3f& v)
{
  unsigned int size = points.size();

  if (size == 0)
    return -1;

  float dist, nearestDist = 10e6; 

  for(PointList::iterator i=points.begin(); i!=points.end(); i++)
    {
      Point* p = *i;
      dist = v.dist2(p->pos);
      if (dist < nearestDist)
	{
	  nearestDist = dist;
	  //index = i;
	}
    }
    
  //return index;
  return 1;
}


void 
PointSet::display(void)
{
#if 0
  glBegin(GL_POINTS);  
  for(PointList::iterator i=points.begin(); i!=points.end(); i++)
    {
      Point* p = *i;
      glColor3f(p->rgb[0], p->rgb[1], p->rgb[2]);
      glNormal3f(p->norm[0], p->norm[1], p->norm[2]);
      glVertex3f(p->pos[0], p->pos[1], p->pos[2]);
    }
  glEnd();
#endif
}

Point * 
PointSet::getNearestPoint (Point * p) {
  Point * res = NULL, *v;
  PointList::iterator i = points.begin();
  float dist, nearestDist = (p->pos).dist2 ((*i)->pos);
  for (;i != points.end(); i++)
    {
      v = *i;
      dist = (p->pos).dist2 (v->pos);
      if (dist < nearestDist)
	{
	  nearestDist = dist;
	  res = v;
	}
    }
  return res;
}
 
