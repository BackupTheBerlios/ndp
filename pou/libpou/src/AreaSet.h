/**
 * @file   AreaSet.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:48:14 2004
 * 
 * @brief  Area set
 *
 * $Log: AreaSet.h,v $
 * Revision 1.7  2004/04/29 08:57:30  leserpent
 * Use a vector for getArea
 *
 * Revision 1.6  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.5  2004/04/28 17:12:33  pumpkins
 * Octree
 *
 * Revision 1.4  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
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

  AreaSet(void): cells() {}
  
  AreaSet(const AreaSet& as): cells(as.cells) {}


  virtual ~AreaSet() {}

  unsigned int size(void) const {
    return cells.size();
  }
  
  int addArea(Area* a) {
    cells.push_back(a);
    return cells.size()-1;
  }

  void remove(unsigned int index) {
    cells.erase(cells.begin() + index);
  }

  void clearAll(void) {
    cells.erase(cells.begin(), cells.end());
  }
  
  //---------------
  
  Area*& operator[](const int index) {
    return cells[index];
  }

  Area* operator[](const int index) const {
    return cells[index];
  }


  void display(void);
  void display(int index);

  // tab is a table of index of Areas that contient p
  // result is a numet of these Areas 
  virtual unsigned int getAreas(const Vec3f& p, std::vector<unsigned int>& tab);

  // flag=true  - copy this[index] to desination
  // flag=false - copy all but this[index] to destination
  void copy(AreaSet& destination, 
	    const std::vector<unsigned int>& index,
	    const bool flag);
};

#endif


