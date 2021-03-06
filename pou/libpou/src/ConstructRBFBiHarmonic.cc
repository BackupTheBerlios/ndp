/**
 * @file   ConstructRBFBiHarmonic.cc
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:52:15 2004
 * 
 * @brief  rbf reconstruction using biharmonics
 *
 * $Log: ConstructRBFBiHarmonic.cc,v $
 * Revision 1.9  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.8  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.7  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.6  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
#include "ConstructRBFBiHarmonic.h"
#include "Solver.h"

#include <iostream>

ConstructRBFBiHarmonic::ConstructRBFBiHarmonic()
{
  c = new float[4];
}

ConstructRBFBiHarmonic::~ConstructRBFBiHarmonic() {
  delete[] c;
}

float
ConstructRBFBiHarmonic::eval(const Vec3f &p) const
{
  if (size == 0)
    return 100;
  
  float sum = 0;
  
  for (unsigned int i = 0; i<size; i++)
    {
      sum += w[i] * phi(dist(p, center[i]));
    }
  
  return sum + c[0]*p[0] + c[1]*p[1] + c[2]*p[2] + c[3];
}

void 
ConstructRBFBiHarmonic::evalGradient(const Vec3f& p, Vec3f& v) const
{
  if (size == 0)
    {
      v[0] = v[1] = v[2] = 0;
      std::cerr << "ImplicitSurface3DrbfBiharmonic::Grad"
		<< " - No Points in Region " << std::endl;
      return;
    }
  
  float tmp;
  v[0] = v[1] = v[2] = 0;
  
  
  for (unsigned int i = 0; i<size; i++)
    {
      
      tmp = w[i] * 1/(dist(p, center[i])) ;
      
      v[0] += tmp * (p[0] - center[i][0]);
      v[1] += tmp * (p[1] - center[i][1]);
      v[2] += tmp * (p[2] - center[i][2]);

    }
  
  v[0] += c[0];
  v[1] += c[1];
  v[2] += c[2];
  
  // Function is positive inside: Normals will point to interior 
  // we have to turn them around
  v[0] =- v[0];
  v[1] =- v[1];
  v[2] =- v[2];
  

}

int 
ConstructRBFBiHarmonic::computeRBF(const ConstraintSet &cs) 
{
  unsigned int size = cs.size();
  double ro = 1e-10;
  //linear system Ax=b  
  Solver s(size+4);
  //Fill matrix A 
  for(unsigned int i=0; i<size; i++)
    for(unsigned int j=i; j<size; j++)
      {
	double val = phi(dist(cs[i]->getVector(), cs[j]->getVector()));
	if (i==j)
	  val -= 8 * size * 3.14f * ro;
	s.setA(i, j, val);
      }
  for(unsigned int i=0; i<size; i++)
    {
      s.setA(i, size+0, cs[i]->getVector()[0]);
      s.setA(i, size+1, cs[i]->getVector()[1]);
      s.setA(i, size+2, cs[i]->getVector()[2]);
      s.setA(i, size+3, 1);
    }

  //Fill matrix b
  for(unsigned int i=0; i<size; i++)
    {
      s.setB(i, cs[i]->getConstraint());
    }
  s.setB(size+0, 0);
  s.setB(size+1, 0);
  s.setB(size+2, 0);
  s.setB(size+3, 0);

  //Solve
  int result = s.solve();

  //Get results into w and polynomial
  setSize(size);
  for(unsigned int i=0; i<size; i++)
    {
      setCenter(i, cs[i]->getVector());
      setW(i, s.getX(i));
    }
      
  setC(s.getX(size),
       s.getX(size+1),
       s.getX(size+2),
       s.getX(size+3));

  return result;  
}

void
ConstructRBFBiHarmonic::setC(const float cx, const float cy, const float cz,
			     const float cg)
{
  c[0] = cx;
  c[1] = cy;
  c[2] = cz;
  c[3] = cg;
}

void
ConstructRBFBiHarmonic::getC(float& cx, float& cy, float& cz, float& cg)
{
  cx = c[0];
  cy = c[1];
  cz = c[2];
  cg = c[3];
}

void
ConstructRBFBiHarmonic::load(std::istream &stream)
{
  unsigned int newSize;

  stream >> newSize;
  setSize(newSize);

  for(unsigned int i = 0; i<newSize; i++)
    {
      float x,y,z,w;
      
      stream >> x >> y >> z >> w;
      setCenter(i,Vec3f(x,y,z));
      setW(i,w);
    }

  float cx,cy,cz,cg;

  stream >> cx >> cy >> cz >> cg;
  setC(cx,cy,cz,cg);
}

void
ConstructRBFBiHarmonic::save(std::ostream &stream) const
{
  stream << size << std::endl;
  for(unsigned int i = 0; i<size; i++)
    stream << getCenter(i)[0] << " " 
	   << getCenter(i)[1] << " " 
	   << getCenter(i)[2] << " " 
	   << getW(i) << std::endl;
  
  stream << c[0] << " "
	 << c[1] << " "
	 << c[2] << " "
	 << c[3] << std::endl;
}
