#ifndef AREASETADAPT_H
#define AREASETADAPT_H

#include "AreaSet.h"
#include "PointSet.h"

#include "vector3.h"
#include "box3d.h"


class AreaSetAdapt: public AreaSet
{
public:
  void create(const PointSet& ps, 
	      unsigned int _threMin,
	      unsigned int _threMax );

private:
  unsigned int threMin;
  unsigned int threMax;
  
  void subdivide(const PointSet& ps, Area* a, bool verbose);
  
};

#endif
