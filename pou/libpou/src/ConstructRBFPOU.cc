/**
 * @file   ConstructRBFPOU.cc
 * @author Arcila Thomas, Dallarosa Damien, Grange Benjamin, Martin Loic
 * @date   Mon Apr  5 20:51:23 2004
 * 
 * @brief  rbf reconstruction using an octree
 *
 * $Log: ConstructRBFPOU.cc,v $
 * Revision 1.21  2004/04/29 16:43:02  leserpent
 * Removed std::cerr<<
 *
 * Revision 1.20  2004/04/29 09:50:17  pumpkins
 * assert
 *
 * Revision 1.19  2004/04/29 09:28:12  leserpent
 * Removed usuless test
 *
 * Revision 1.18  2004/04/29 08:57:30  leserpent
 * Use a vector for getArea
 *
 * Revision 1.17  2004/04/27 13:38:42  pumpkins
 * missing header
 *
 * Revision 1.16  2004/04/27 13:01:12  pumpkins
 * marching cubes portable
 * constructrbfpou asserts instead of exception
 *
 * Revision 1.15  2004/04/27 08:57:18  leserpent
 * abort ok
 *
 * Revision 1.14  2004/04/26 08:05:22  pumpkins
 * gradian->gradient
 *
 * Revision 1.13  2004/04/25 15:19:48  pumpkins
 * callback fixes
 * exception fixes
 * multiple constructrbfpou compute allowed
 *
 * Revision 1.12  2004/04/25 12:09:24  pumpkins
 * error throw std::runtime_exception
 * Compute(POU) throw logic_error if (cs.size < threMin)
 *
 * Revision 1.11  2004/04/20 11:16:38  pumpkins
 * gzstream
 * authors
 * models
 *
 * Revision 1.10  2004/04/06 16:49:32  leserpent
 * ConstructRBFPOU::setcallback() can take two more parameters in order
 * to show only one progress dialog during color computing.
 *
 * Revision 1.9  2004/04/05 19:14:36  pumpkins
 * File documentation
 * 
 * 
 */
#include "ConstructRBFPOU.h"
#include "AreaCube.h"
#include "AreaSphere.h"
#include "AreaSetOctree.h"
#include <iostream>
#include <cassert>

ConstructRBFPOU::ConstructRBFPOU(ConstructRBFPOU::TypeRBF type):
    type(type), threMin(50), threMax(100), overlap(0.5f), callback(0), step(1)
{
  cells = new AreaSetOctree();
  cf=const_cast<ConstraintFilter*>(ConstructRBF::NULL_FILTER);
}

ConstructRBFPOU::~ConstructRBFPOU() {
  delete cells;
}

bool
ConstructRBFPOU::compute(ConstraintSet& cs, const AreaSet *octree)
  throw (std::logic_error)
{
  int result;

  applyFilter(cs);
  assert(cs.size() >= threMin);

  dynamic_cast<AreaSetOctree *>(cells)->create(cs, threMin, threMax, overlap);
  unsigned int size=cells->size();

  rbf.clear();
  for(unsigned int i=0; i<size; i++)
    {
      AreaSphere area(*dynamic_cast<AreaSphere*>((*cells)[i]));

      if (i % step == 0 && callback)
	if (!callback(i+pass*size, size*numPass))
	  return 0;

      
      while (1)
	{
	  ConstraintSet filtered(cs, &area);
	  float size = filtered.size();
	  
	  if (size<threMin)
	    area.grow(0.05);
	  else if (size>threMax)
	    area.reduce(0.02);
	  else
	    break;
	}

      ConstraintSet filtered(cs, &area);
      
      ConstructRBF* newrbf = newRBF(); 

      if (filtered.size() != 0) 
	  result = newrbf->compute(filtered);
        
      rbf.push_back (newrbf);
      flag.push_back (OK_FLAG);
    }
  return 1;
}

void
ConstructRBFPOU::setThresholds(unsigned int _threMin, unsigned _threMax)
{
  assert (_threMin < _threMax);
  threMin = _threMin;
  threMax = _threMax;
}

ConstructRBF *
ConstructRBFPOU::newRBF()
{
  switch(type)
    {
    case BIHARMONIC:
      return new ConstructRBFBiHarmonic();
    case TRIHARMONIC:
      return new ConstructRBFTriHarmonic();
    case THINPLATE:
      return new ConstructRBFThinPlate();
    default:
      return new ConstructRBFBiHarmonic();
    }
}

float
ConstructRBFPOU::eval(const Vec3f &p) const
{
  std::vector<unsigned int> tab;
  float sum=0, sumW=0;
  
  cells->getAreas(p, tab);

  while(!tab.empty()) {
    unsigned int i = tab.back();
    tab.pop_back();

    float w=(*cells)[i]->w(p);
    float s=rbf[i]->eval(p);
    
    sumW+=w;
    sum+=s * w;
  }

  if (sumW == 0)
    return -1e8;
  else
    return sum/sumW;
}

void
ConstructRBFPOU::evalGradient(const Vec3f &p, Vec3f &v) const
{
  std::vector<unsigned int> tab;
  Vec3f localNormal, localWd;
  float sumWdX = 0 , sumWdY = 0, sumWdZ = 0 ;
  float sumfdWX = 0, sumfdWY = 0, sumfdWZ = 0 ;
  float sumfWdX =0, sumfWdY = 0, sumfWdZ = 0;

  cells->getAreas(p, tab);
  
  float sum=0, sumW=0;
 
  while(!tab.empty()) {
    unsigned int i = tab.back();
    tab.pop_back();
    
    float w = (*cells)[i]->w(p);      
    (*cells)[i]->wd(p, localWd);      
      
    float f = rbf[i]->eval(p);
    rbf[i]->evalGradient(p, localNormal);
      
    sumW += w;
    sum += f * w;
      
    sumWdX += localWd.x;
    sumWdY += localWd.y;
    sumWdZ += localWd.z;
      
    sumfdWX += localNormal[0] * w;
    sumfdWY += localNormal[1] * w;
    sumfdWZ += localNormal[2] * w;
      
    sumfWdX += f * localWd.x;
    sumfWdY += f * localWd.y;
    sumfWdZ += f * localWd.z;
  }

  if (sumW == 0) 
      v.setValues(0,0,0);
  else 
    {
      v.setValues((sumW * (sumfdWX + sumfWdX) - sumWdX * sum) / (sumW * sumW),
                  (sumW * (sumfdWY + sumfWdY) - sumWdY * sum) / (sumW * sumW),
                  (sumW * (sumfdWZ + sumfWdZ) - sumWdZ * sum) / (sumW * sumW));
  
      v.setValues(sumfdWX/sumW, sumfdWY/sumW, sumfdWZ/sumW);
      
    }
}

void
ConstructRBFPOU::evalNormal(const Vec3f &p, Vec3f &v) const
{
  evalGradient(p, v);
  v.normalize();
}

void
ConstructRBFPOU::load(std::istream &stream)
{
  int intType;
  stream >> intType;
  
  switch(intType)
    {
    case 0:
      type = BIHARMONIC;
      break;
    case 1:
      type = TRIHARMONIC;
      break;
    case 2:
      type = THINPLATE;
      break;
    }

  stream >> threMin >> threMax;

  unsigned int newNbArea;

  stream >> newNbArea;

  for(unsigned int i = 0; i<newNbArea; i++)
    {
      ConstructRBF* newrbf = newRBF(); 
      newrbf->load(stream);
      rbf.push_back(newrbf);
    }
  
  cells = new AreaSet();
  for(unsigned int i = 0; i<newNbArea; i++)
    {
      float cx, cy, cz, r;
      stream >> cx >> cy >> cz >> r;

      if (r>0)
	cells->addArea(new AreaSphere(Vec3f(cx, cy, cz), r));
      else
	{
	  float x2,y2,z2;
	  stream >> x2 >> y2 >> z2;
	  cells->addArea(new AreaCube(cx, cy, cz, x2, y2, z2));
	}
    }
}

void
ConstructRBFPOU::save(std::ostream &stream) const
{
  switch(type)
    {
    case BIHARMONIC:
      stream << 0 << std::endl;
      break;
    case TRIHARMONIC:
      stream << 1 << std::endl;
      break;
    case THINPLATE:
      stream << 2 << std::endl;
      break;
    }

  stream << threMin << " " << threMax << std::endl;

  unsigned int size = rbf.size();
  stream << size << std::endl;

  for(unsigned int i = 0; i<size; i++)
    {
      rbf[i]->save(stream);
    }

  for(unsigned int i = 0; i<size; i++)
    {
      (*cells)[i]->save(stream);
    }
}
