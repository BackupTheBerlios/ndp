#include "ConstructRBFPOU.h"
#include "AreaCube.h"
#include "AreaSphere.h"
#include "AreaSetOctree.h"
#include <fstream>
#include <iostream>

ConstructRBFPOU::ConstructRBFPOU(ConstructRBFPOU::TypeRBF _type)
{
  cells = new AreaSetOctree();
  type = _type;
  threMin = 50;
  threMax = 100;
  overlap=0.5f;
  step=1;
  callback=0;
  cf=const_cast<ConstraintFilter*>(ConstructRBF::NULL_FILTER);
}

ConstructRBFPOU::~ConstructRBFPOU() {
  delete cells;
}

void
ConstructRBFPOU::compute(ConstraintSet& cs, const AreaSet *octree)
{
  int result;
  applyFilter(cs);
  if (octree)
    cells=const_cast<AreaSet *>(octree);
  else {
    dynamic_cast<AreaSetOctree *>(cells)->create(cs, threMin, threMax, overlap);

  unsigned int size=cells->size();
  std::cerr << "octree fini" << std::endl;
  for(unsigned int i=0; i<size; i++)
    {
      AreaSphere area(*dynamic_cast<AreaSphere*>((*cells)[i]));

      if (i % step == 0 && callback)
	callback(i, size);

      std::cerr << "Region " << i << "/" << cells->size()
                << " --> " << std::flush;

      while(1)
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

      /*BUG: Ne doit pas filtrer les contraintes geometriques!=0*/
      ConstraintSet filtered(cs, &area);
      std::cerr << filtered.size() << " points "<< std::endl << std::flush;
      
      ConstructRBF* newrbf = newRBF(); 
      /*      newrbf->setProjDist(_projDist);
	      newrbf->setProjValidate(_projValidateFlag);*/
      

      if (filtered.size() != 0) 
	{
	  result = newrbf -> compute(filtered);
	}
        
      rbf.push_back(newrbf);
      flag.push_back(OK_FLAG);
    }
  }
}

void
ConstructRBFPOU::setThresholds(unsigned int _threMin, unsigned _threMax)
{
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
  unsigned int size = rbf.size();
  unsigned int nbIntersect;

  unsigned int *tab = new unsigned int[size];
  
  nbIntersect = cells->getAreas(p, tab);

  float sum=0, sumW=0;

  for (unsigned int i=0; i<nbIntersect; i++) {
    if (!rbf[tab[i]])
      continue;

    float w = (*cells)[tab[i]]->w(p);
    float s = rbf[tab[i]]->eval(p);
    
    sumW += w;
    sum += s * w;
  }
  if (sumW == 0)
    std::cerr << "WARNING: in eval sumW = 0" << std::endl;
  delete[] tab;
  if (sumW == 0)
    return -1e8;
  else
    return sum/sumW;
}

void
ConstructRBFPOU::evalGradian(const Vec3f &p, Vec3f &v) const
{
  unsigned int size = rbf.size();
  unsigned int nbIntersect;
  unsigned int * tab = new unsigned int[size];
  Vec3f localNormal, localWd;
  float sumWdX = 0 , sumWdY = 0, sumWdZ = 0 ;
  float sumfdWX = 0, sumfdWY = 0, sumfdWZ = 0 ;
  float sumfWdX =0, sumfWdY = 0, sumfWdZ = 0;

  nbIntersect = cells->getAreas(p, tab);
  
  float sum=0, sumW=0;
  
  for(unsigned int i=0; i<nbIntersect; i++)
    {
      if (!rbf[tab[i]])
	continue;

      float w = (*cells)[tab[i]]->w(p);      
      (*cells)[tab[i]]->wd(p, localWd);      
      
      float f = rbf[tab[i]]->eval(p);
      rbf[tab[i]]->evalGradian(p, localNormal);
      //float f = (i+1)*p.x()*p.x() + p.y()*p.y() + p.z()*p.z();
      //localNormal.setValues(2*p.x()*(i+1), 2*p.y(), 2*p.z());
      
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
      
      //cout << "f " << i << " --> " << f << endl;
      
      
    }

  delete [] tab;
  if (sumW == 0) 
    {
      v.setValues(0,0,0);
      // cerr << "WARNING: in Grad sumW = 0 " << endl;
    }
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
  evalGradian(p, v);
  v.normalize();
}

void
ConstructRBFPOU::load(std::ifstream &stream)
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
ConstructRBFPOU::save(std::ofstream &stream) const
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
