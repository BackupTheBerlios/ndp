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
