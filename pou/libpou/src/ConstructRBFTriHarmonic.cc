/**
 * @file   ConstructRBFTriHarmonic.cc
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:43:17 2004
 * 
 * @brief  rbf reconstruction using triharmonics
 * 
 * $Log: ConstructRBFTriHarmonic.cc,v $
 * Revision 1.7  2004/04/20 11:16:39  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.6  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */

#include "ConstructRBFTriHarmonic.h"
#include "Solver.h"
#include <iostream>

#include <fstream>

ConstructRBFTriHarmonic::ConstructRBFTriHarmonic()
{
  c = new float[10];
}

ConstructRBFTriHarmonic::~ConstructRBFTriHarmonic()
{
  delete[] c;
}


int
ConstructRBFTriHarmonic::computeRBF(const ConstraintSet &cs) 
{
  unsigned int size = cs.size();
  double ro = 1e-12;

  //linear system Ax=b
  Solver s(size+10);

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
      s.setA(i, size+0, cs[i]->getVector()[0]*cs[i]->getVector()[0]);
      s.setA(i, size+1, cs[i]->getVector()[1]*cs[i]->getVector()[1]);
      s.setA(i, size+2, cs[i]->getVector()[2]*cs[i]->getVector()[2]);

      s.setA(i, size+3, cs[i]->getVector()[0]*cs[i]->getVector()[1]);
      s.setA(i, size+4, cs[i]->getVector()[1]*cs[i]->getVector()[2]);
      s.setA(i, size+5, cs[i]->getVector()[2]*cs[i]->getVector()[0]);

      s.setA(i, size+6, cs[i]->getVector()[0]);
      s.setA(i, size+7, cs[i]->getVector()[1]);
      s.setA(i, size+8, cs[i]->getVector()[2]);

      s.setA(i, size+9, 1);
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
  s.setB(size+4, 0);
  s.setB(size+5, 0);
  s.setB(size+6, 0);
  s.setB(size+7, 0);
  s.setB(size+8, 0);
  s.setB(size+9, 0);
  
  //Solve
  int result = s.solve();
  
  //Get results into w and polynomial
  setSize(size);
  
  for(unsigned int i=0; i<size; i++)
    {
      setCenter(i, cs[i]->getVector());
      setW(i, s.getX(i));
    }

  c[0] = s.getX(size);
  c[1] = s.getX(size+1);
  c[2] = s.getX(size+2);
  c[3] = s.getX(size+3);
  c[4] = s.getX(size+4);
  c[5] = s.getX(size+5);
  c[6] = s.getX(size+6);
  c[7] = s.getX(size+7);
  c[8] = s.getX(size+8);
  c[9] = s.getX(size+9);

  return result;
}

float
ConstructRBFTriHarmonic::eval(const Vec3f& p) const
{
  if (size == 0)
    return 100;

  float sum = 0;

  for (unsigned int i = 0; i<size; i++)
    {
      sum += w[i] * phi(dist(p, center[i]));
    }

  return sum
    + c[0]*p[0]*p[0] + c[1]*p[1]*p[1] + c[2]*p[2]*p[2]
    + c[3]*p[0]*p[1] + c[4]*p[1]*p[2] + c[5]*p[2]*p[0]
    + c[6]*p[0] + c[7]*p[1] + c[8]*p[2]
    + c[9];
}

float
ConstructRBFTriHarmonic::getL(const BoxVolume& box) const {
  if (size == 0)
    {
      std::cerr << "ImplicitSurface3DrbfTriharmonic::L - No Points in Region " << std::endl;
      return 0;
    }
  float d, tmp;
  float sum = 0;
  Vec3f min, max;
  box.getBounds(min, max);

  for (unsigned int i = 0; i < size; i++)
    {
      Vec3f farestPoint(
			fabs(min.x - center[i].x) > fabs(max.x - center[i].x) ? min.x : max.x,
			fabs(min.y - center[i].y) > fabs(max.y - center[i].y) ? min.x : max.y,
			fabs(min.z - center[i].z) > fabs(max.z - center[i].z) ? min.x : max.z);
      d = (dist(farestPoint, center[i]));
      tmp = fabs(d*d * w[i] * 3);
      sum += tmp;
    }
  
  return sum;
  
}

void
ConstructRBFTriHarmonic::evalGradian(const Vec3f &p, Vec3f &v) const
{  
  if (size == 0)
    {
      v[0] = v[1] = v[2] = 0;
      std::cerr << "ImplicitSurface3DrbfTriharmonic::Grad - No Points in Region " << std::endl;
      return;
    }
  float d, tmp;

  v[0] = v[1] = v[2] = 0;

  for (unsigned int i = 0; i < size; i++)
    {
      d = (dist(p, center[i]));
      tmp = d * w[i] * 3;
      v[0] += tmp * (p[0] - center[i][0]);
      v[1] += tmp * (p[1] - center[i][1]);
      v[2] += tmp * (p[2] - center[i][2]);
    }
  
  v[0] += 2 * c[0] * p[0]  +   c[3] * p[1] + c[5] * p[2] + c[6] ;
  v[1] += 2 * c[1] * p[1]  +   c[3] * p[0] + c[4] * p[2] + c[7] ;
  v[2] += 2 * c[2] * p[2]  +   c[4] * p[1] + c[5] * p[0] + c[8] ;

  // Function is positive inside: Normals will point to interior 
  // we have to turn them around
  v[0] =- v[0];
  v[1] =- v[1];
  v[2] =- v[2];
}

void
ConstructRBFTriHarmonic::jacobi(const Vec3f& p, Vec3f& lx, Vec3f& ly, Vec3f& lz) const
{
  if (size == 0)
    {
      lx.setValues(0.0, 0.0, 0.0);
      ly = lz = lx;
      std::cerr << "ImplicitSurface3DrbfTriharmonic::Jacobi - No Points in Region " << std::endl;
      return;
    }
  
  lx.setValues(0.0, 0.0, 0.0);
  ly = lz =lx;
  
  for (unsigned int i = 0; i<size; i++)
    {
      double wi_dist      = w[i] * dist(p, center[i]);
      double wi_dist_div  = (double) w[i] * (1.0 / dist(p, center[i]));
		
      Vec3f diff = p - center[i];         
      // dfxx
      lx[0] = 3*(wi_dist_div * diff.x * diff.x + wi_dist) + 2 * c[0];
      // dfxy
      lx[1] = 3*(wi_dist_div * diff.y * diff.x) + c[3];
      // dfxz
      lx[2] = 3*(wi_dist_div * diff.z * diff.x) + c[5];
      // dfyx
      ly[0] = lx[1];
      // dfyy
      ly[1] = 3*(wi_dist_div * diff.y * diff.y + wi_dist) + 2 * c[1];
      // dfyz
      ly[2] = 3*(wi_dist_div * diff.z * diff.y) + c[4];
      // dfzx
      lz[0] = lx[2];
      // dfzy
      lz[1] = ly[2];
      // dfzz
      lz[2] = 3*(wi_dist_div * diff.z * diff.z + wi_dist) + 2 * c[2];
    }
}

void ConstructRBFTriHarmonic::load(std::istream &stream) {
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
  
  stream >> c[0] >> c[1] >> c[2]
	 >> c[3] >> c[4] >> c[5]
    	 >> c[6] >> c[7] >> c[8] >> c[9];
}

void ConstructRBFTriHarmonic::save(std::ostream &stream) const {
  stream << size << std::endl;

  for(unsigned int i = 0; i<size; i++)
    stream << getCenter(i)[0] << " " 
	   << getCenter(i)[1] << " " 
	   << getCenter(i)[2] << " " 
	   << getW(i) << std::endl;
  
  stream << c[0] << " "
	 << c[1] << " "
	 << c[2] << " "	 
	 << c[3] << " "
	 << c[4] << " "
	 << c[5] << " "
	 << c[6] << " "
	 << c[7] << " "
	 << c[8] << " "
	 << c[9] << std::endl;
}
