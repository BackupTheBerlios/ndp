#ifndef CONSTRAINTFILTER_H
#define CONSTRAINTFILTER_H

#include <vector>

class ConstraintFilter {
 public:
  virtual void filter(std::vector<Constraint>& cs, int parm) = 0;
};

#endif /* CONSTRAINTFILTER_H */
