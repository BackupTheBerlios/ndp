/**
 * @file   AreaSetOctree.cc
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:54:02 2004
 * 
 * @brief  Area set construction, octree based
 * 
 * 
 */
#include "AreaSetOctree.h"
#include "AreaSphere.h"
#include "AreaCube.h"
#include <queue>
#include <fstream>
#include <assert.h>
#include <algorithm>


// THIS CODE IS NOT CLEAN !!!

// I HOPE IT RUNS BUT
// IT NEEDS TO BE REWRITEN !!!

static const unsigned int OCTREE_BUFFER_SIZE = 256;
  
void 
AreaSetOctree::create(const ConstraintSet& cs,
		      unsigned int _threMin,
		      unsigned int _threMax,
		      const float _overlap)

{
  threMin = _threMin;
  threMax = _threMax;
  overlap = _overlap;

  BoxVolume box = cs.getBoundingBox();

  Vec3f boxSize;
  box.getSize(boxSize);
  float maxVal = boxSize.maxValue() / 2.0;  

  
  Vec3f center;
  box.getCenter(center);
    
  // bounding box will be a cube
  Vec3f min = center - Vec3f(maxVal, maxVal, maxVal);
  Vec3f max = center + Vec3f(maxVal, maxVal, maxVal);
  box = BoxVolume(min,max);

  // create octree
  subdivide(root, cs, box);
  
  // set maxLevel
  maxLevel = getMaxLevel();
}


void
AreaSetOctree::subdivide(Node* &node,
			 const ConstraintSet& cs,
			 const BoxVolume& box)
{
  node = new Node(box, 2*overlap);
  ConstraintSet filter(cs, node->a);
  unsigned int size = filter.size();

  if (size>threMax)
    {
      Vec3f boxMin = box.getMin();      
      Vec3f boxMax = box.getMax();
      Vec3f boxCenter;
      box.getCenter(boxCenter);

      BoxVolume newBox;

      //can we really simplify these lines ???

      newBox = BoxVolume(boxMin[0], boxMin[1], boxMin[2],
			 boxCenter[0], boxCenter[1], boxCenter[2]);
      subdivide(node->child[0], cs, newBox);

      newBox = BoxVolume(boxMin[0], boxMin[1], boxCenter[2],
			 boxCenter[0], boxCenter[1], boxMax[2]);
      subdivide(node->child[1], cs, newBox);

      newBox = BoxVolume(boxMin[0], boxCenter[1], boxMin[2],
			 boxCenter[0], boxMax[1], boxCenter[2]);
      subdivide(node->child[2], cs, newBox);

      newBox = BoxVolume(boxMin[0], boxCenter[1], boxCenter[2],
			 boxCenter[0], boxMax[1], boxMax[2]);
      subdivide(node->child[3], cs, newBox);

      newBox = BoxVolume(boxCenter[0], boxMin[1], boxMin[2],
			 boxMax[0], boxCenter[1], boxCenter[2]);
      subdivide(node->child[4], cs, newBox);

      newBox = BoxVolume(boxCenter[0], boxMin[1], boxCenter[2],
			 boxMax[0], boxCenter[1], boxMax[2]);
      subdivide(node->child[5], cs, newBox);

      newBox = BoxVolume(boxCenter[0], boxCenter[1], boxMin[2],
			 boxMax[0], boxMax[1], boxCenter[2]);
      subdivide(node->child[6], cs, newBox);

      newBox = BoxVolume(boxCenter[0], boxCenter[1], boxCenter[2],
			 boxMax[0], boxMax[1], boxMax[2]);
      subdivide(node->child[7], cs, newBox);
    }
  else if (size<threMin)
    {
      node->areaIndex = addArea(node->a);
      return;
    }
  else //number of points OK
    {
      //this case is very easy :-)
      node->areaIndex = addArea(node->a);
      return;
    }
  return;
}

void 
AreaSetOctree::saveOctree(const char * filename) {
  std::ofstream stream (filename);
  saveOctree (stream);
  stream.close ();
}

void 
AreaSetOctree::saveOctree(std::ostream& stream)
{
  root->save(stream);
}

void 
AreaSetOctree::Node::save(std::ostream& stream)
{
  bool isLeaf = 
    (child[0] == 0) &&
    (child[1] == 0) &&
    (child[2] == 0) &&
    (child[3] == 0) &&
    (child[4] == 0) &&
    (child[5] == 0) &&
    (child[6] == 0) &&
    (child[7] == 0);

  if (isLeaf)
    {
      stream << -1 << " ";
      stream << box.getMin()[0] << " "
	     << box.getMin()[1] << " "
	     << box.getMin()[2] << " "
	     << box.getMax()[0] << " "
	     << box.getMax()[1] << " "
	     << box.getMax()[2] << " ";
      stream << areaIndex << " ";
      stream << 1 << std::endl;
    }
   else
     {
       stream << -2 << std::endl;
       stream << box.getMin()[0] << " "
	      << box.getMin()[1] << " "
	      << box.getMin()[2] << " "
	      << box.getMax()[0] << " "
	      << box.getMax()[1] << " "
	      << box.getMax()[2] << std::endl;
       child[0]->save(stream);
       child[1]->save(stream);
       child[2]->save(stream);
       child[3]->save(stream);
       child[4]->save(stream);
       child[5]->save(stream);
       child[6]->save(stream);
       child[7]->save(stream);
       stream << 2 << std::endl;
     }
}

void 
AreaSetOctree::loadOctree(const char * filename) {
  std::ifstream stream (filename);
  loadOctree (stream);
  stream.close ();
}

void
AreaSetOctree::loadOctree(std::istream& stream)
{
  root = Node::load(stream);
}

AreaSetOctree::Node*
AreaSetOctree::Node::load(std::istream& stream)
{
  Node* node;
  int tagBegin, tagEnd;
  stream >> tagBegin;

  float x0, x1, y0, y1, z0, z1;
  stream >> x0 >> y0 >> z0
	 >> x1 >> y1 >> z1;
  node = new Node(BoxVolume(Vec3f(x0,y0,z0),Vec3f(x1,y1,z1)));
  

  if (tagBegin == -2)
    {
      node->child[0] = load(stream);
      node->child[1] = load(stream);
      node->child[2] = load(stream);
      node->child[3] = load(stream);
      node->child[4] = load(stream);
      node->child[5] = load(stream);
      node->child[6] = load(stream);
      node->child[7] = load(stream);
    }
  else
    {
      stream >> node->areaIndex;
    }
  stream >> tagEnd;
  return node;
}

int
AreaSetOctree::getMaxLevel(void)
{
  return maxLevelRec(root);
}

int
AreaSetOctree::maxLevelRec(AreaSetOctree::Node* node)
{
   int max, level;
   
   if (node->isLeaf ())
     return 1;
   else
     {
       max = 0;
       for (int i = 0; i < 8; i++) {
	 if (node->child[i] != 0) {
             level = maxLevelRec(node->child[i]);
             if (level > max) {
	       max = level;
             };
	 }; 
	 
       }
       
       return 1+max;
     }
}

unsigned int
AreaSetOctree::getAreas(const Vec3f& p, std::vector<unsigned int>& tab)
{
  std::vector<Node*> q;
  q.reserve (OCTREE_BUFFER_SIZE);
  unsigned int front = 0;
  q.push_back (root);
  int count=0;

  while (front < q.size ()) 
    {
      Node * node = q[front];
      assert (node != NULL);
      if (node->isLeaf ())
	{
	  if (node->a->intersect (p))
	    tab.push_back (node->areaIndex);
	}
      else 
	for (unsigned int i = 0; i < 8; i++)
	  if (node->child[i] != NULL && node->child[i]->a->intersect (p)) 
	    q.push_back (node->child[i]) ;
      front++;
    }
  return tab.size();
}

void AreaSetOctree::getIntersectionList (const Vec3f& origin,
					 const Vec3f& direction, 
					 IntersectionVector & vec)
{
  std::vector<Node*> q;
  unsigned int front = 0;
  float depth1 = 0.0, depth2 = 0.0;
  
  q.reserve (OCTREE_BUFFER_SIZE);
  q.push_back (root);
 
  while (front < q.size ()) 
    {
      Node * node = q[front];
      assert (node != NULL);
      if (node->isLeaf ())
	{
	  if (node->a->intersectRay (origin, direction, depth1, depth2)) {
	    vec.push_back (Intersection (depth1, true, node));
	    vec.push_back (Intersection (depth2, false, node));
	  } 
	}
      else 
	for (unsigned int i = 0; i < 8; i++)
	  if (node->child[i] != NULL && node->child[i]->a->intersectRay (origin, direction, depth1, depth2)) 
	    q.push_back (node->child[i]) ;
      front++;
    }
  assert (vec.size () % 2 == 0);
  sort (vec.begin (), vec.end ());
}  
  
bool 
operator< (const AreaSetOctree::Intersection & i1, 
	   const AreaSetOctree::Intersection & i2)
{
  return (i1.getDepth () < i2.getDepth ());
}
