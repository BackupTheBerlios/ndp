#ifndef CONSTRAINTSET_H
#define CONSTRAINTSET_H

#include <vector>
#include "Area.h"
#include "vector3.h"
#include "box3d.h"
#include "Constraint.h"

class ConstraintSet
{
 private:
  std::vector<Constraint *> constraints;
  BoxVolume box;
    
 public:
  ConstraintSet (): constraints (), box () {}
  ConstraintSet (const ConstraintSet & cs);
  ConstraintSet (const ConstraintSet & cs, const Area * a);

  void load (const char *filename);
  void save (const char *filename);

  Constraint *operator[] (int i) const;
  std::vector<Constraint *>::const_iterator getBegin () const;
  std::vector<Constraint *>::const_iterator getEnd () const;

  std::vector<Constraint *>::iterator getBegin ();
  std::vector<Constraint *>::iterator getEnd ();

  void add (Constraint * p);
  unsigned int size () const;


  void removeAll ();
  void remove (std::vector<Constraint *>::iterator i);
  const BoxVolume & getBoundingBox () const;
};

inline Constraint *
ConstraintSet::operator[] (int i) const
{
  return constraints[i];
}

inline const BoxVolume &
ConstraintSet::getBoundingBox () const
{
  return box;
}
inline void
ConstraintSet::remove (std::vector < Constraint * >::iterator i)
{
  constraints.erase (i);
}

inline void
ConstraintSet::removeAll ()
{
  constraints.clear ();
}

inline unsigned int
ConstraintSet::size (void) const
{
  return constraints.size ();
}

inline std::vector<Constraint *>::const_iterator
ConstraintSet::getBegin () const
{
  return constraints.begin ();
}

inline std::vector<Constraint *>::const_iterator
ConstraintSet::getEnd () const
{
  return constraints.end ();
}

inline std::vector<Constraint *>::iterator
ConstraintSet::getBegin ()
{
  return constraints.begin ();
}

inline std::vector<Constraint *>::iterator
ConstraintSet::getEnd ()
{
  return constraints.end ();
}

inline void
ConstraintSet::add (Constraint * p)
{
  constraints.push_back (p);
  box.extendBy (p->getVector ());
}

#endif
