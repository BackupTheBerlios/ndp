#ifndef FUNCTION_W_H
#define FUNCTION_W_H

#include <iostream>

//C0
inline float w0(float r)
{
  return 1-r;
}

inline float w0d(float r)
{
  return -1;
}

//C1
inline float w1(float r)
{
  
  return r*r*(2*r-3) + 1; 
}


inline float w1d(float r)
{
  //w1=2r^3-3r^2+1
  if (r>1)
    {
      return 0;
      std::cout << "w1d r>1" << std::endl;
    }
  return 6*r*(r-1); 
}

//C2
inline float w2(float r)
{
  float r2 = r*r;
  float r3 = r2*r;
  float r4 = r2*r2;
  float r5 = r3*r2;

  return -6*r5 + 15*r4 - 10*r3 +1; 
}

//C2
inline float w2d(float r)
{
  float r2 = r*r;
  float r3 = r2*r;
  float r4 = r2*r2;

  return -30*r4 + 60*r3 - 30*r2; 
}
//C2
inline float wtest(float r)
{
  return (1-r)*(1-r);
}

//C2
inline float wtestd(float r)
{
  return 2*r-2;  
}

//Christophe/Carole IS'95
inline float wChristophe(float r)
{
  const float p = 1;
  float r2 = r*r;
  return r<0.5 ? 
    1-(9*r2*r2)/(p+(4.5-4*p)*r2) :
    (1-r2)*(1-r2)/(0.75-p+(1.5+4*p)*r2);
}

#endif
