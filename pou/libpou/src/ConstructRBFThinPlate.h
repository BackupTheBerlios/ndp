/**
 * @file   ConstructRBFThinPlate.h
 * @author Arcila Thomas
 * @date   Mon Apr  5 20:42:28 2004
 * 
 * @brief  rbf reconstruction using thinplate
 *
 * $Log: ConstructRBFThinPlate.h,v $
 * Revision 1.2  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#ifndef CONSTRUCTRBFTHINPLATE_H
#define CONSTRUCTRBFTHINPLATE_H

#include "ConstructRBF.h"
#include "box3d.h"

class ConstructRBFThinPlate : public ConstructRBFBiHarmonic {
 protected:
  virtual inline double phi(const double r) const
    {
      if (r)
	return r*r*log(r);
      return 0;
    }

};

#endif /* CONSTRUCTRBFTHINPLATE_H */
