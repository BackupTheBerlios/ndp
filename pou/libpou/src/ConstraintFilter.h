/**
 * @file   ConstraintFilter.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:45:34 2004
 * 
 * @brief  Filter a ConstraintSet
 * 
 * $Log: ConstraintFilter.h,v $
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
#ifndef CONSTRAINTFILTER_H
#define CONSTRAINTFILTER_H

#include <vector>

class ConstraintFilter {
 public:
  virtual void filter(ConstraintSet& cs, int parm) = 0;
};

#endif /* CONSTRAINTFILTER_H */
