#ifndef AREASETGRID_H
#define AREASETGRID_H

#include "AreaSet.h"
#include "PointSet.h"

#include "vector3.h"
#include "box3d.h"


class AreaSetGrid: public AreaSet
{
private:
public:
  void create(const PointSet& ps, 
	      unsigned int sizeX,
	      unsigned int sizeY,
	      unsigned int sizeZ,
	      unsigned int overlap);
};

#endif
