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
