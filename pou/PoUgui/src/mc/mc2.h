/*$Log: mc2.h,v $
/*Revision 1.2  2004/04/01 20:38:41  leserpent
/*Moved private methods from mc2.h to mc2.cc
/*Removed useless typedef.
/**/
#ifndef MC_H
#define MC_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ImplicitSurface3D.h"

#include "math/vector3.h"
#include "box3d.h"

class Mc {
private:
  std::vector<unsigned int> indices;
  void (*progress_callback)(int v, int max);
  unsigned int progress_step;
  unsigned int progress_max;
  ImplicitSurface3D *is;

public:
  Mc(void (*callback)(int, int), int step);
  
  void mc_setcallback(void (*c)(int, int), unsigned int step) {
    progress_callback = c;
    progress_step = step;
  }

  const std::vector<unsigned int>& getIndices();
  void getVertNorm(std::vector<Vec3f> &vertices, std::vector<Vec3f> &normals);
  void domc(ImplicitSurface3D *imps, const Box3f &bbox);

private:
  int RES; /* # converge iterations    */

  enum Direction {L=0,R,B,T,N,F};
  enum Corner {LBN=0,LBF,LTN,LTF,RBN,RBF,RTN,RTF};
  enum {LB=0, LT, LN, LF, RB, RT, RN, RF, BN, BF, TN, TF} edge;

  static const unsigned int HASHBIT=5;
  static const unsigned int HASHSIZE=1<<(3*HASHBIT);
  static const unsigned int MASK=(1<<HASHBIT)-1;

  struct POINT {		   /* a three-dimensional point */
    double x, y, z;		   /* its coordinates */
  };

  struct TEST {		   /* test the function for a signed value */
    POINT p;			   /* location of test */
    double value;		   /* function value at p */
    int ok;			   /* if value is of correct sign */
  };

  struct VERTEX {		   /* surface vertex */
    POINT position, normal;	   /* position and surface normal */
  };

  struct VERTICES {	   /* list of vertices in polygonization */
    int count, max;		   /* # vertices, max # allowed */
    VERTEX *ptr;		   /* dynamically allocated */
  };

  struct CORNER {		   /* corner of a cube */
    int i, j, k;		   /* (i, j, k) is index within lattice */
    double x, y, z, value;	   /* location and function value */
  };

  struct CUBE {		   /* partitioning cell (cube) */
    int i, j, k;		   /* lattice location of cube */
    CORNER *corners[8];		   /* eight corners */
  };

  struct CUBES {		   /* linked list of cubes acting as stack */
    CUBE cube;			   /* a single cube */
    CUBES *next;		   /* remaining elements */
  };

  struct CENTERLIST {	   /* list of cube locations */
    int i, j, k;		   /* cube location */
    CENTERLIST *next;	   /* remaining elements */
  };

  struct CORNERLIST {	   /* list of corners */
    int i, j, k;		   /* corner id */
    double value;		   /* corner value */
    CORNERLIST *next;	   /* remaining elements */
  };

  struct EDGELIST {	   /* list of edges */
    int i1, j1, k1, i2, j2, k2;	   /* edge corner ids */
    int vid;			   /* vertex id */
    EDGELIST *next;	   /* remaining elements */
  };

  struct INTLIST {	   /* list of integers */
    int i;			   /* an integer */
    INTLIST *next;	   /* remaining elements */
  };

  struct INTLISTS {	   /* list of list of integers */
    INTLIST *list;		   /* a list of integers */
    INTLISTS *next;	   /* remaining elements */
  };

  struct PROCESS {	   /* parameters, function, storage */
    double size, delta;		   /* cube size, normal delta */
    int bounds;			   /* cube range within lattice */
    POINT start;		   /* start point on surface */
    CUBES *cubes;		   /* active cubes */
    VERTICES vertices;		   /* surface vertices */
    CENTERLIST **centers;	   /* cube center hash table */
    CORNERLIST **corners;	   /* corner value hash table */
    EDGELIST **edges;		   /* edge and vertex id hash table */
  };


/**** Cubical Polygonization (optional) ****/
  int gntris;	     /* global needed by application */
  VERTICES gvertices;  /* global needed by application */


  INTLISTS *cubetable[256];

  static const Corner corner1[12];
  static const Corner corner2[12];
  static const Direction leftface[12];
  static const Direction rightface[12];

  double RAND();
  int HASH(int i, int j, int k);
  int BIT(int i, int bit);
  int FLIP(int i, int bit);

  int triangle (int i1, int i2, int i3, VERTICES vertices);
  char *polygonize (double size, int bounds, double x, double y, double z, 
                        bool enableTet);
  void testface (int i, int j, int k, CUBE *old, int face, int c1, int c2,
                     int c3, int c4, PROCESS *p);
  CORNER *setcorner (PROCESS *p, int i, int j, int k);
  TEST find (int sign, PROCESS *p, double x, double y, double z);
  int dotet (CUBE *cube, int c1, int c2, int c3, int c4, PROCESS *p);
  int docube (CUBE *cube, PROCESS * p);
  int nextcwedge (int edge, int face);
  int otherface (int edge, int face);
  void makecubetable (void);
  void *mycalloc (int nitems, int nbytes);
  int setcenter(CENTERLIST **table, int i, int j, int k);
  void setedge (EDGELIST **table, int i1, int j1, int k1, int i2, int j2,
                    int k2, int vid);
  int getedge (EDGELIST **table, int  i1, int j1, int  k1, int  i2, int  j2,int  k2);
  int vertid (CORNER* c1, CORNER* c2, PROCESS* p);
  void addtovertices (VERTICES *vertices,VERTEX v);
  void vnormal (POINT* point, PROCESS* p, POINT* v);
  void converge (POINT* p1, POINT* p2, double v,   POINT* p);
  double fun(double x, double y, double z);
};


#endif
