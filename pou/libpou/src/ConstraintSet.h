/**
 * @file   ConstraintSet.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:45:15 2004
 * 
 * @brief  Constraint set support
 * 
 * $Log: ConstraintSet.h,v $
 * Revision 1.8  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.7  2004/04/25 12:09:24  pumpkins
 * error throw std::runtime_exception
 * Compute(POU) throw logic_error if (cs.size < threMin)
 *
 * Revision 1.6  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.5  2004/04/07 08:01:14  leserpent
 * Added a	constructor which takes a PointSet.
 * Removed an useless copy constructor.
 *
 * Revision 1.4  2004/04/06 16:14:26  leserpent
 * Added a removeDeleteAll() method.
 *
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#ifndef CONSTRAINTSET_H
#define CONSTRAINTSET_H

#include <vector>
#include <stdexcept>
#include "Area.h"
#include "math/vector3.h"
#include "helpers/deletor.h"
#include "box3d.h"
#include "Constraint.h"

class PointSet;

class ConstraintSet
{
 private:
  std::vector<Constraint *> constraints;
  BoxVolume box;
    
 public:
  ConstraintSet::ConstraintSet () {}
  ConstraintSet::ConstraintSet (const PointSet &ps);
  ConstraintSet (const ConstraintSet & cs, const Area * a);
  void load (const char *filename) throw (std::runtime_error);

  void save (const char *filename) throw (std::runtime_error);

  Constraint *operator[] (int i) const {
    return constraints[i];
  }

  std::vector<Constraint *>::const_iterator begin () const {
      return constraints.begin ();
  }

  std::vector<Constraint *>::const_iterator end () const {
    return constraints.end ();
  }

  std::vector<Constraint *>::iterator begin () {
    return constraints.begin ();
  }

  std::vector<Constraint *>::iterator end () {
    return constraints.end ();
  }

  void add (Constraint * p) {
    constraints.push_back (p);
    box.extendBy (p->getVector ());
  }

  unsigned int size () const {
    return constraints.size ();
  }


  void removeAll () {
    constraints.clear ();
  }

  void removeDeleteAll() {
    for_each(constraints.begin(), constraints.end(), DeleteObject());
      constraints.clear();
  }
  void remove (std::vector<Constraint *>::iterator i) {
      constraints.erase (i);
  }

  const BoxVolume & getBoundingBox () const {
      return box;
  }
};

#endif
