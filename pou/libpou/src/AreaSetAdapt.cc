#include "AreaSetAdapt.h"
#include "AreaSphere.h"
#include "AreaCube.h"
#include "vector3.h"
#include "box3d.h"


using namespace std;
  
void 
AreaSetAdapt::create(const PointSet& ps,
		     unsigned int _threMin,
		     unsigned int _threMax)

{
  threMin = _threMin;
  threMax = _threMax;

  BoxVolume box = ps.getBoundingBox();

  Vec3f boxSize;
  box.getSize(boxSize);
  float maxVal = boxSize.maxValue() / 2.0;  

  Vec3f center;
  box.getCenter(center);
    
  // bounding box will be a cube
  Vec3f min = center - Vec3f(maxVal, maxVal, maxVal);
  Vec3f max = center + Vec3f(maxVal, maxVal, maxVal);

  box = BoxVolume(min,max);
  //box = BoxVolume(-1,-1,-1,1,1,1);

  // NEED TO BE SIMPLIFIED !!!...
  box.getCenter(center);
  box.getSize(boxSize);
  maxVal = boxSize.maxValue(); 
  float radius = boxSize.length() / 2;
  
  // first reconstruction now
  Area* boundingArea = new AreaSphere(center, radius);
  //Vec3f bs = boxSize * 0.5 * 1.2;
  //Area* boundingArea = new AreaCube(center-bs, center+bs);
  subdivide(ps, boundingArea, false);
}


void
AreaSetAdapt::subdivide(const PointSet& ps, 
			Area* a, 
			bool verbose)
{
  //static int threMax = 120;
  //static int threMin = 40;

  PointSet filter(ps, a);
  unsigned int size = filter.size();

  if (size==0)
    {
      //cout << "No points "<< endl;
      return;
    }
  else if (size>threMax)
    {
      //cout << "Too many points, subdividing" << endl;
      Area** area8 = a->subdivide();
      
      subdivide(filter, area8[0], verbose);
      subdivide(filter, area8[1], verbose);
      subdivide(filter, area8[2], verbose);
      subdivide(filter, area8[3], verbose);
      subdivide(filter, area8[4], verbose);
      subdivide(filter, area8[5], verbose);
      subdivide(filter, area8[6], verbose);
      subdivide(filter, area8[7], verbose);
      /*
      subdivide(ps, area8[0], verbose);
      subdivide(ps, area8[1], verbose);
      subdivide(ps, area8[2], verbose);
      subdivide(ps, area8[3], verbose);
      subdivide(ps, area8[4], verbose);
      subdivide(ps, area8[5], verbose);
      subdivide(ps, area8[6], verbose);
      subdivide(ps, area8[7], verbose);
      */
      delete[] area8;
    }
  else if (size<threMin)
    {
      //cout << "More points need, growing area" << endl;
      
      a->grow(0.1);
      subdivide(ps, a, verbose);
    }
  else
    {
      //OK
      if (verbose)
	cout << size << " points --> OK, adding area " << a << endl;
      addArea(a);
    }

  return;
}


