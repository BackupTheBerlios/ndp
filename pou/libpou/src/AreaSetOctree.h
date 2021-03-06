/**
 * @file   AreaSetOctree.h
 * @author Irek Tobor, Patrik Reuter
 * @date   Mon Apr  5 20:47:36 2004
 * 
 * @brief  Area set construction, octree based
 * 
 * $Log: AreaSetOctree.h,v $
 * Revision 1.10  2004/04/29 08:57:30  leserpent
 * Use a vector for getArea
 *
 * Revision 1.9  2004/04/28 19:27:37  pumpkins
 * hash_set removal
 *
 * Revision 1.8  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.7  2004/04/28 17:12:33  pumpkins
 * Octree
 *
 * Revision 1.6  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#ifndef AREASETOCTREE_H
#define AREASETOCTREE_H

#include "AreaSet.h"
#include "AreaSphere.h"
#include "ConstraintSet.h"

#include <fstream>
#include <vector>
#include "math/vector3.h"
#include "box3d.h"

class AreaSetOctree: public AreaSet
{
public:
  class Node
    {
    public:
      Node* child[8];
      BoxVolume box;
      Area* a;
      int areaIndex;
      std::vector < int > neighborIndices;
      
      Node(const BoxVolume& b, float grow = 1.0f):
	box(b)
	{
	  child[0] = child[1] = child[2] = child[3] =
	    child[4] = child[5] = child[6] = child[7] = NULL;
	  
	  a = new AreaSphere(b, grow);
	}
      
      void save(std::ostream& stream);    
      static Node* load(std::istream& stream);    
      
      inline bool isLeaf () const { return  ((child[0] == 0) &&
					     (child[1] == 0) &&
					     (child[2] == 0) &&
					     (child[3] == 0) &&
					     (child[4] == 0) &&
					     (child[5] == 0) &&
					     (child[6] == 0) &&
					     (child[7] == 0));
      }
    };
  
  
  class Intersection
    {
    private:
      float depth;
      bool enter;
      Node * node;
      
    public:
      Intersection (float t = 0.0, bool enter  = true, Node * n = NULL) 
	: depth (t), enter (enter), node (n) {}
      
      virtual ~Intersection () {}
      
      inline float getDepth () const { return depth;}
      inline bool isEnter () const { return enter;}
      inline Node * getNode () const { return node;}  
      
    };
  
  typedef std::vector<Intersection> IntersectionVector;
  typedef std::vector<unsigned int> AreaIndexVector;
  
 public:
  void create(const ConstraintSet& cs,
	      unsigned int _threMin,
	      unsigned int _threMax,
	      float overlap);
  
  void saveOctree(const char * filename);
  void saveOctree(std::ostream& stream);
  void loadOctree(const char * filename);
  void loadOctree(std::istream& stream);
  
  int getMaxLevel(void);
  int maxLevelRec(AreaSetOctree::Node* node);
  
  void fillOctreeTable(void);
  void fillOctreeTableRec(AreaSetOctree::Node* node, int level);
  
  /// Fill tab with all the leaf node's index of the octree containing p.
  unsigned int getAreas(const Vec3f& p, std::vector<unsigned int>& tab);
  
  /// fill vec with all the intersections of a ray (defined by origin and direction)  with the octree
  void getIntersectionList (const Vec3f& origin, 
			    const Vec3f& direction, 
			    IntersectionVector & vec);
  inline BoxVolume getBBox () const { return root->box;}
 
 private:
  unsigned int threMin;
  unsigned int threMax;
  float overlap;
  Node* root;
  unsigned int maxLevel;  
  
  void subdivide(Node* &node, const ConstraintSet& ps, const BoxVolume& box );
};


/// For sorting purpose.
extern bool operator< (const AreaSetOctree::Intersection & i1,
		       const AreaSetOctree::Intersection & i2);

#endif
