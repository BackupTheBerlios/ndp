/**
 * @file   ConstraintSet.cc
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:53:20 2004
 * 
 * @brief  ConstraintSet management
 * 
 * $Log: ConstraintSet.cc,v $
 * Revision 1.4  2004/04/06 16:14:26  leserpent
 * Added a removeDeleteAll() method.
 *
 * Revision 1.3  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#include <fstream>
#include <iostream>
#include "ConstraintSet.h"
#include "math/vector3.h"
#include "box3d.h"

ConstraintSet::ConstraintSet (const ConstraintSet &cs):
  constraints (cs.constraints),
  box (cs.box)
{
}


ConstraintSet::ConstraintSet (const ConstraintSet &cs, const Area *a):
  constraints ()
{
  std::vector<Constraint *>::const_iterator i;
  for (i = cs.constraints.begin (); i != cs.constraints.end (); i++) {
    Constraint *c = *i;

    if (a->intersect (c->getVector ())) {
      add (c);
    }
  }
}


void
ConstraintSet::load (const char *filename)
{
  std::ifstream fs (filename);
  int counter = 0;

  while (!fs.eof ()) {
    float x, y, z, c;

    fs >> x >> y >> z >> c;

    if (!fs.eof ()) {
      Constraint *p = new Constraint (Vec3f (x, y, z), c);

      add (p);
      counter++;
    }
  }

  fs.close ();
}


void
ConstraintSet::save (const char *filename)
{
  std::ofstream fs (filename);

  for (std::vector<Constraint *>::iterator i = constraints.begin ();
       i != constraints.end (); i++) {
    Constraint *p = *i;

    fs << p->getVector ()[0] << " "
       << p->getVector ()[1] << " "
       << p->getVector ()[2] << " " << p->getConstraint () << std::endl;
  }

  fs.close ();
}
