#include "AreaSetGrid.h"
#include "AreaSphere.h"
#include "AreaCube.h"
#include "vector3.h"
#include "box3d.h"


using namespace std;
  
void 
AreaSetGrid::create(const PointSet& ps, 
		    unsigned int sizeX,
		    unsigned int sizeY,
		    unsigned int sizeZ,
		    unsigned int overlap)
{
  BoxVolume box = ps.getBoundingBox();
  
  Vec3f boxMin = box.getMin();
  Vec3f boxMax = box.getMax();

  Vec3f boxSize;
  box.getSize(boxSize);

  Vec3f step(boxSize[0]/sizeX, boxSize[0]/sizeY, boxSize[0]/sizeZ);
  //  float radius = step.length() / 2 * (1+overlap/100.0f);

  for(unsigned int i=0; i<sizeX; i++)
    for(unsigned int j=0; j<sizeY; j++)
      for(unsigned int k=0; k<sizeZ; k++)
	{
	  Vec3f center = 
	    boxMin + 
	    step * 0.5 + 
	    Vec3f(step[0] * i,
		  step[1] * j,
		  step[2] * k);

	  
	  //Area* a = new AreaSphere(center, radius);

	  Vec3f b0 = center - (step * 0.5 * (1+overlap/100.0f));
	  Vec3f b1 = center + (step * 0.5 * (1+overlap/100.0f));
	  Area *a = new AreaCube(b0,b1);


	  PointSet psFiltered(ps, a);
	  if (psFiltered.size() > 5)
	    cells.push_back(a);
	}
}



