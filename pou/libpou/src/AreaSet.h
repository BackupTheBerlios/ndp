#ifndef AREASET_H
#define AREASET_H

#include <vector>
#include "Area.h"
#include "math/vector3.h"
#include "box3d.h"


class AreaSet
{
protected:
  std::vector<Area*> cells;

public:
  int numero;

  AreaSet(void):
    cells()
  {
  }
  
  AreaSet(const AreaSet& as):
    cells(as.cells)
  {
  }


  virtual ~AreaSet()
  {
  }

  unsigned int size(void) const
  {
    return cells.size();
  }
  
  int addArea(Area* a)
  {
    cells.push_back(a);
    return cells.size()-1;
  }

  void remove(unsigned int index)
  {
    cells.erase(cells.begin() + index);
  }

  void clearAll(void)
  {
    cells.erase(cells.begin(), cells.end());
  }
  
  //---------------

  //---------------
  
  Area*& operator[](const int index)
  {
    return cells[index];
  }

  Area* operator[](const int index) const
  {
    return cells[index];
  }


  void display(void);
  void display(int index);

  // tab is a table of index of Areas that contient p
  // result is a numet of these Areas 
  unsigned int getAreas(const Vec3f& p, unsigned int* tab);

  // flag=true  - copy this[index] to desination
  // flag=false - copy all but this[index] to destination
  void copy(AreaSet& destination, 
	    const std::vector<unsigned int>& index,
	    const bool flag);

};

#endif


