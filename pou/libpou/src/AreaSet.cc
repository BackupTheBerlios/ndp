#include "AreaSet.h"
#include "vector3.h"
#include "box3d.h"

using namespace std;

unsigned int 
AreaSet::getAreas(const Vec3f& p, unsigned int* tab)
{
  unsigned int size = cells.size();
  unsigned int counter = 0;

  for(unsigned int i=0; i<size; i++)
    {
      if (cells[i]->intersect(p))
        {
          tab[counter] = i;
          counter++;
        }
    }
 
  return counter;
}

void 
AreaSet::copy(AreaSet& destination, 
	      const vector<unsigned int>& index,
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


void 
AreaSet::display(void)
{
  int size = cells.size();
  for(int i = 0; i<size; i++)
    {
      cells[i]->display();
    }
}


void 
AreaSet::display(int index)
{
  cells[index]->display();
}


