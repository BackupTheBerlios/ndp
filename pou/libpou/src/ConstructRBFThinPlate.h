/**
 * @file   ConstructRBFThinPlate.h
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:42:28 2004
 * 
 * @brief  rbf reconstruction using thinplate
 *
 * $Log: ConstructRBFThinPlate.h,v $
 * Revision 1.4  2004/04/27 14:19:12  pumpkins
 * normals in the right direction
 *
 * Revision 1.3  2004/04/20 11:16:39  pumpkins
 * gzstream
 * authors
 * models
 *
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
	return -r*r*log(r);
      return 0;
    }

};

#endif /* CONSTRUCTRBFTHINPLATE_H */
