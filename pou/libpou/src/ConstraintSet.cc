/**
 * @file   ConstraintSet.cc
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:53:20 2004
 * 
 * @brief  ConstraintSet management
 * 
 * $Log: ConstraintSet.cc,v $
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
#include <fstream>
#include <iostream>
#include "helpers/gzstream.h"
#include "PointSet.h"
#include "ConstraintSet.h"
#include "math/vector3.h"
#include "box3d.h"

ConstraintSet::ConstraintSet (const PointSet &ps):
  constraints (), box ()
{
  PointList::const_iterator end=ps.getEnd();
  
  for(PointList::const_iterator i=ps.getBegin(); i!=end; ++i)
    add(new Constraint((*i)->getPos()));
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
 throw (std::runtime_error)
{
  igzstream fs (filename);
  if (!fs)
    throw std::runtime_error ("ConstraintSet file cannot be opened for "
			      "reading\n");
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
  throw (std::runtime_error)
{
  ogzstream fs (filename);
  if (!fs)
    throw std::runtime_error ("ConstraintSet file cannot be opened for "
			      "writing\n");

  for (std::vector<Constraint *>::iterator i = constraints.begin ();
       i != constraints.end (); i++) {
    Constraint *p = *i;

    fs << p->getVector ()[0] << " "
       << p->getVector ()[1] << " "
       << p->getVector ()[2] << " " << p->getConstraint () << std::endl;
  }

  fs.close ();
}
