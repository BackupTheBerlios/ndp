/**
 * @file   AreaSet.cc
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:54:34 2004
 * 
 * @brief  Area set
 * 
 * $Log: AreaSet.cc,v $
 * Revision 1.7  2004/04/29 09:28:12  leserpent
 * Removed usuless test
 *
 * Revision 1.6  2004/04/29 08:57:30  leserpent
 * Use a vector for getArea
 *
 * Revision 1.5  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#include <vector>

#include "AreaSet.h"
#include "math/vector3.h"
#include "box3d.h"

unsigned int 
AreaSet::getAreas(const Vec3f& p, std::vector<unsigned int>& tab)
{
  unsigned int size = cells.size();

  for(unsigned int i=0; i<size; i++)
      if (cells[i]->intersect(p))
          tab.push_back(i);
 
  return tab.size();
}

void 
AreaSet::copy(AreaSet& destination, 
	      const std::vector<unsigned int>& index,
	      const bool flag)
{
  if (flag)
    {
      for(unsigned int i=0; i<index.size(); i++)
	{
	  destination.addArea(cells[i]);
	}
    }
  else
    {
      for(unsigned int i=0; i<size(); i++)
	{
	  bool is = false;
	  for(unsigned int j=0; j<index.size(); j++)
	    {
	      if (index[j]==i)
		{
		  is = true;
		  break;
		}
	    }
	  if (!is)
	    destination.addArea(cells[i]);
	}
    }
}
