#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>

#include "mc2.h"
#include "ImplicitSurface3D.h"

Mc::Mc(void (*callback)(int, int), int step):
  cubeSize(0.05), maxIt(10), tetActive(false), progress_step(step)
{
  assert(callback);
  progress_callback = callback;
  initPoint = Vec3f(-0.269614, 0.228464, 0.0772211);
  memset(cubetable, 0, 256*sizeof(int));
  gvertices.ptr = NULL;
}

Mc::~Mc() {
  clear();
}

// Free resulting vertices of a previous call to domc
void Mc::clear() {
  indices.clear();
  gntris = 0;
  free((char *) gvertices.ptr);
  gvertices.ptr = NULL;
  gvertices.max = 0;
  gvertices.count = 0;
}

void Mc::domc(ImplicitSurface3D *imps, const Box3f &bbox)
{
  is = imps;

  char *err;

  clear();
  
//BUG: The Pointset bounding box doesn't seem to be a good bounding
//	box for the surface
  Vec3f size=bbox.getSize();
  int bounds = (int)(size.maxValue()/(2*0.05))+20;

  progress_max = (int)((size.x/cubeSize)*(size.y/cubeSize)*(size.z/cubeSize)/8);
  if ((err = polygonize(cubeSize, 200/*bounds*/, 
                        initPoint.x, initPoint.y, initPoint.z,
                        tetActive)) != NULL) 
  {
    std::cerr << "Error " << err << std::endl;
  }

  std::cerr << gntris << " triangles, " 
            << gvertices.count << "  vertices\n" << std::endl; 
}

const std::vector<unsigned int>& Mc::getIndices() {
  return indices;
}

void Mc::getPoints(std::vector<Point> &points) {
  for (int i = 0; i < gvertices.count; i++) {
    VERTEX v;
    Vec3f pos, normal, color;
    v = gvertices.ptr[i];

    pos.setValues(v.position.x, v.position.y, v.position.z);
    normal.setValues(v.normal.x, v.normal.y, v.normal.z);
    is->evalColorRGB(pos, color);
    points.push_back(Point(pos, normal, color));
  }
}


/*****************Wrapped code*********************************************/
/*
 * C code from the article
 * "An Implicit Surface Polygonizer"
 * by Jules Bloomenthal, jbloom@beauty.gmu.edu
 * in "Graphics Gems IV", Academic Press, 1994
 */

/* implicit.c
 *     an implicit surface polygonizer, translated from Mesa
 *     applications should call polygonize()
 *
 * to compile a test program for ASCII output:
 *     cc implicit.c -o implicit -lm
 *
 * to compile a test program for display on an SGI workstation:
 *     cc -DSGIGFX implicit.c -o implicit -lgl_s -lm
 *
 * Authored by Jules Bloomenthal, Xerox PARC.
 * Copyright (c) Xerox Corporation, 1991.  All rights reserved.
 * Permission is granted to reproduce, use and distribute this code for
 * any and all purposes, provided that this notice appears in all copies. */
double Mc::RAND() {
  return (rand()&32767)/32767.0;
}

int Mc::HASH(int i, int j, int k) {
  return ((((i&MASK)<<HASHBIT)|(j&MASK))<<HASHBIT)|(k&MASK);
}

int Mc::BIT(int i, int bit) {
  return (i>>bit)&1;
}

int Mc::FLIP(int i, int bit) {
  return i^1<<bit;
}


/* triangle: called by polygonize() for each triangle; write to stdout */

int Mc::triangle (int i1, int i2, int i3, VERTICES vertices)
{
  gvertices = vertices;
  gntris++;

  indices.push_back(i1);
  indices.push_back(i2);
  indices.push_back(i3);
    
  return 1;
}


/**** An Implicit Surface Polygonizer ****/


/* polygonize: polygonize the implicit surface function
 *   arguments are:
 *	 double function (x, y, z)
 *		 double x, y, z (an arbitrary 3D point)
 *	     the implicit surface function
 *	     return negative for inside, positive for outside
 *	 double size
 *	     width of the partitioning cube
 *	 int bounds
 *	     max. range of cubes (+/- on the three axes) from first cube
 *	 double x, y, z
 *	     coordinates of a starting point on or near the surface
 *	     may be defaulted to 0., 0., 0.
 *	 int triproc (i1, i2, i3, vertices)
 *		 int i1, i2, i3 (indices into the vertex array)
 *		 VERTICES vertices (the vertex array, indexed from 0)
 *	     called for each triangle
 *	     the triangle coordinates are (for i = i1, i2, i3):
 *		 vertices.ptr[i].position.x, .y, and .z
 *	     vertices are ccw when viewed from the out (positive) side
 *		 in a left-handed coordinate system
 *	     vertex normals point outwards
 *	     return 1 to continue, 0 to abort
 *	 int mode
 *	     TET: decompose cube and polygonize six tetrahedra
 *	     NOTET: polygonize cube directly
 *   returns error or NULL
 */
char *Mc::polygonize (double size, 
                      int bounds, 
                      double x, double y, double z, 
                      bool enableTet)
{
  PROCESS p;
  int n, noabort;
  unsigned int i=0;
  TEST in, out;

  memset(&p, 0, sizeof(PROCESS));
  p.size = size;
  p.bounds = bounds;
  p.delta = size/(double)(maxIt*maxIt);
  
  /* allocate hash tables and build cube polygon table: */
  p.centers = (CENTERLIST **) mycalloc(HASHSIZE,sizeof(CENTERLIST *));
  p.corners = (CORNERLIST **) mycalloc(HASHSIZE,sizeof(CORNERLIST *));
  p.edges =	(EDGELIST   **) mycalloc(2*HASHSIZE,sizeof(EDGELIST *));
  makecubetable();
  
  /* find point on surface, beginning search at (x, y, z): */
  srand(1);
  in = find(1, &p, x, y, z);
  out = find(0, &p, x, y, z);
  if (!in.ok || !out.ok) return "can't find starting point";
  converge(&in.p, &out.p, in.value, &p.start);
  
  /* push initial cube on stack: */
  p.cubes = (CUBES *) mycalloc(1, sizeof(CUBES)); /* list of 1 */
  p.cubes->cube.i = p.cubes->cube.j = p.cubes->cube.k = 0;
  p.cubes->next = NULL;

  /* set corners of initial cube: */
  for (n = 0; n < 8; n++)
    p.cubes->cube.corners[n] = setcorner(&p, BIT(n,2), BIT(n,1), BIT(n,0));
  
  p.vertices.count = p.vertices.max = 0; /* no vertices yet */
  p.vertices.ptr = NULL;
  
  setcenter(p.centers, 0, 0, 0);

  while (p.cubes != NULL) { /* process active cubes till none left */
    i++;
    CUBE c;
    CUBES *temp = p.cubes;
    c = p.cubes->cube;
    
    noabort = enableTet?
              /* either decompose into tetrahedra and polygonize: */
              dotet(&c, LBN, LTN, RBN, LBF, &p) &&
              dotet(&c, RTN, LTN, LBF, RBN, &p) &&
              dotet(&c, RTN, LTN, LTF, LBF, &p) &&
              dotet(&c, RTN, RBN, LBF, RBF, &p) &&
              dotet(&c, RTN, LBF, LTF, RBF, &p) &&
              dotet(&c, RTN, LTF, RTF, RBF, &p)
              :
              /* or polygonize the cube directly: */
              docube(&c, &p);
    if (! noabort) return "aborted";
    
    /* pop current cube from stack */
    p.cubes = p.cubes->next;
    free((char *) temp);
    /* test six face directions, maybe add to stack: */
    testface(c.i-1, c.j, c.k, &c, L, LBN, LBF, LTN, LTF, &p);
    testface(c.i+1, c.j, c.k, &c, R, RBN, RBF, RTN, RTF, &p);
    testface(c.i, c.j-1, c.k, &c, B, LBN, LBF, RBN, RBF, &p);
    testface(c.i, c.j+1, c.k, &c, T, LTN, LTF, RTN, RTF, &p);
    testface(c.i, c.j, c.k-1, &c, N, LBN, LTN, RBN, RTN, &p);
    testface(c.i, c.j, c.k+1, &c, F, LBF, LTF, RBF, RTF, &p);

    if(i%progress_step==0)
      if(i<progress_max)
        progress_callback(i,progress_max);
      else
        progress_callback(99, 100);
  }
  progress_callback(100, 100);

  return NULL;
}


/* testface: given cube at lattice (i, j, k), and four corners of face,
 * if surface crosses face, compute other four corners of adjacent cube
 * and add _new cube to cube stack */

void Mc::testface (int i, int j, int k, 
                   CUBE *old, int face, 
                   int c1, int c2, int c3, int c4, PROCESS *p)
{
  CUBE _new;
  CUBES *oldcubes = p->cubes;
  static int facebit[6] = {2, 2, 1, 1, 0, 0};
  int n, pos = old->corners[c1]->value > 0.0 ? 1 : 0, bit = facebit[face];

  /* test if no surface crossing, cube out of bounds, or already visited: */
  if ((old->corners[c2]->value > 0) == pos &&
      (old->corners[c3]->value > 0) == pos &&
      (old->corners[c4]->value > 0) == pos) return;
  if (abs(i) > p->bounds || abs(j) > p->bounds || abs(k) > p->bounds) return;
  if (setcenter(p->centers, i, j, k)) return;

  /* create _new cube: */
  _new.i = i;
  _new.j = j;
  _new.k = k;
  for (n = 0; n < 8; n++) _new.corners[n] = NULL;
  _new.corners[FLIP(c1, bit)] = old->corners[c1];
  _new.corners[FLIP(c2, bit)] = old->corners[c2];
  _new.corners[FLIP(c3, bit)] = old->corners[c3];
  _new.corners[FLIP(c4, bit)] = old->corners[c4];
  for (n = 0; n < 8; n++)
    if (_new.corners[n] == NULL)
      _new.corners[n] = setcorner(p, i+BIT(n,2), j+BIT(n,1), k+BIT(n,0));

  /*add cube to top of stack: */
  p->cubes = (CUBES *) mycalloc(1, sizeof(CUBES));
  p->cubes->cube = _new;
  p->cubes->next = oldcubes;
}


/* setcorner: return corner with the given lattice location
   set (and cache) its function value */

Mc::CORNER *Mc::setcorner (PROCESS *p, int i, int j, int k)
{
  /* for speed, do corner value caching here */
  CORNER *c = (CORNER *) mycalloc(1, sizeof(CORNER));
  int index = HASH(i, j, k);
  CORNERLIST *l = p->corners[index];
  c->i = i; c->x = p->start.x+((double)i-.5)*p->size;
  c->j = j; c->y = p->start.y+((double)j-.5)*p->size;
  c->k = k; c->z = p->start.z+((double)k-.5)*p->size;
  for (; l != NULL; l = l->next)
    if (l->i == i && l->j == j && l->k == k) {
      c->value = l->value;
      return c;
    }
  l = (CORNERLIST *) mycalloc(1, sizeof(CORNERLIST));
  l->i = i; l->j = j; l->k = k;
  l->value = c->value = fun(c->x, c->y, c->z);
  l->next = p->corners[index];
  p->corners[index] = l;
  return c;
}


/* find: search for point with value of given sign (0: neg, 1: pos) */

Mc::TEST Mc::find (int sign, PROCESS *p, double x, double y, double z)
{
  int i;
  TEST test;
  double range = p->size;
  test.ok = 1;
  for (i = 0; i < 10000; i++) {
    test.p.x = x+range*(RAND()-0.5);
    test.p.y = y+range*(RAND()-0.5);
    test.p.z = z+range*(RAND()-0.5);
    test.value = fun(test.p.x, test.p.y, test.p.z);
    if (sign == (test.value > 0.0)) return test;
    range = range*1.0005; /* slowly expand search outwards */
  }
  test.ok = 0;
  return test;
}


/**** Tetrahedral Polygonization ****/


/* dotet: triangulate the tetrahedron
 * b, c, d should appear clockwise when viewed from a
 * return 0 if client aborts, 1 otherwise */

int Mc::dotet (CUBE *cube, 
               int c1, int c2, int c3, int c4, 
               PROCESS *p)
{
  CORNER *a = cube->corners[c1];
  CORNER *b = cube->corners[c2];
  CORNER *c = cube->corners[c3];
  CORNER *d = cube->corners[c4];
  int index = 0, apos, bpos, cpos, dpos, e1, e2, e3, e4, e5, e6;
  if ((apos = (a->value > 0.0))) index += 8;
  if ((bpos = (b->value > 0.0))) index += 4;
  if ((cpos = (c->value > 0.0))) index += 2;
  if ((dpos = (d->value > 0.0))) index += 1;
  /* index is now 4-bit number representing one of the 16 possible cases */
  if (apos != bpos) e1 = vertid(a, b, p);
  if (apos != cpos) e2 = vertid(a, c, p);
  if (apos != dpos) e3 = vertid(a, d, p);
  if (bpos != cpos) e4 = vertid(b, c, p);
  if (bpos != dpos) e5 = vertid(b, d, p);
  if (cpos != dpos) e6 = vertid(c, d, p);
  /* 14 productive tetrahedral cases (0000 and 1111 do not yield polygons */
  switch (index) {
  case 1:	 return triangle(e5, e6, e3, p->vertices);
  case 2:	 return triangle(e2, e6, e4, p->vertices);
  case 3:	 return triangle(e3, e5, e4, p->vertices) &&
                   triangle(e3, e4, e2, p->vertices);
  case 4:	 return triangle(e1, e4, e5, p->vertices);
  case 5:	 return triangle(e3, e1, e4, p->vertices) &&
                   triangle(e3, e4, e6, p->vertices);
  case 6:	 return triangle(e1, e2, e6, p->vertices) &&
                   triangle(e1, e6, e5, p->vertices);
  case 7:	 return triangle(e1, e2, e3, p->vertices);
  case 8:	 return triangle(e1, e3, e2, p->vertices);
  case 9:	 return triangle(e1, e5, e6, p->vertices) &&
                   triangle(e1, e6, e2, p->vertices);
  case 10: return triangle(e1, e3, e6, p->vertices) &&
             triangle(e1, e6, e4, p->vertices);
  case 11: return triangle(e1, e5, e4, p->vertices);
  case 12: return triangle(e3, e2, e4, p->vertices) &&
             triangle(e3, e4, e5, p->vertices);
  case 13: return triangle(e6, e2, e4, p->vertices);
  case 14: return triangle(e5, e3, e6, p->vertices);
  }
  return 1;
}

/* docube: triangulate the cube directly, without decomposition */

int Mc::docube (CUBE *cube, PROCESS * p)
{
  INTLISTS *polys;
  int i, index = 0;
  for (i = 0; i < 8; i++) if (cube->corners[i]->value > 0.0) index += (1<<i);
  for (polys = cubetable[index]; polys; polys = polys->next) {
    INTLIST *edges;
    int a = -1, b = -1, count = 0;
    for (edges = polys->list; edges; edges = edges->next) {
      CORNER *c1 = cube->corners[corner1[edges->i]];
      CORNER *c2 = cube->corners[corner2[edges->i]];
      int c = vertid(c1, c2, p);
      if (++count > 2 && ! triangle(a, b, c, p->vertices)) return 0;
      if (count < 3) a = b;
      b = c;
    }
  }
  return 1;
}


/* nextcwedge: return next clockwise edge from given edge around given face */

int Mc::nextcwedge (int edge, int face)
{
  switch (edge) {
  case LB: return (face == L)? LF : BN;
  case LT: return (face == L)? LN : TF;
  case LN: return (face == L)? LB : TN;
  case LF: return (face == L)? LT : BF;
  case RB: return (face == R)? RN : BF;
  case RT: return (face == R)? RF : TN;
  case RN: return (face == R)? RT : BN;
  case RF: return (face == R)? RB : TF;
  case BN: return (face == B)? RB : LN;
  case BF: return (face == B)? LB : RF;
  case TN: return (face == T)? LT : RN;
  case TF: return (face == T)? RT : LF;
  }
  return 0;
}


/* otherface: return face adjoining edge that is not the given face */

int Mc::otherface (int edge, int face)
{
  int other = leftface[edge];
  return face == other? rightface[edge] : other;
}


/* makecubetable: create the 256 entry table for cubical polygonization */

void Mc::makecubetable (void) {
  int i, e, c, done[12], pos[8];
  for (i = 0; i < 256; i++) {
    for (e = 0; e < 12; e++) done[e] = 0;
    for (c = 0; c < 8; c++) pos[c] = BIT(i, c);
    for (e = 0; e < 12; e++)
      if (!done[e] && (pos[corner1[e]] != pos[corner2[e]])) {
        INTLIST *ints = 0;
        INTLISTS *lists = (INTLISTS *) mycalloc(1, sizeof(INTLISTS));
        int start = e, edge = e;
        /* get face that is to right of edge from pos to neg corner: */
        int face = pos[corner1[e]]? rightface[e] : leftface[e];
        while (1) {
          edge = nextcwedge(edge, face);
          done[edge] = 1;
          if (pos[corner1[edge]] != pos[corner2[edge]]) {
            INTLIST *tmp = ints;
            ints = (INTLIST *) mycalloc(1, sizeof(INTLIST));
            ints->i = edge;
            ints->next = tmp; /* add edge to head of list */
            if (edge == start) break;
            face = otherface(edge, face);
          }
        }
        lists->list = ints; /* add ints to head of table entry */
        lists->next = cubetable[i];
        cubetable[i] = lists;
      }
  }
}


/**** Storage ****/


/* mycalloc: return successful calloc or exit program */

void *Mc::mycalloc (int nitems, int nbytes)
{
  void *ptr = calloc(nitems, nbytes);
  if (ptr != NULL) return ptr;
  fprintf(stderr, "can't calloc %d bytes\n", nitems*nbytes);
  exit(1);
}


/* setcenter: set (i,j,k) entry of table[]
 * return 1 if already set; otherwise, set and return 0 */

int Mc::setcenter(CENTERLIST **table, int i, int j, int k)
{
  int index = HASH(i, j, k);
  CENTERLIST *_new, *l, *q = table[index];
  for (l = q; l != NULL; l = l->next)
    if (l->i == i && l->j == j && l->k == k) return 1;
  _new = (CENTERLIST *) mycalloc(1, sizeof(CENTERLIST));
  _new->i = i; _new->j = j; _new->k = k; _new->next = q;
  table[index] = _new;
  return 0;
}


/* setedge: set vertex id for edge */

void Mc::setedge (EDGELIST **table, 
                  int i1, int j1, int k1, 
                  int i2, int j2, int k2, 
                  int vid)
{
  unsigned int index;
  EDGELIST *_new;
  if (i1>i2 || (i1==i2 && (j1>j2 || (j1==j2 && k1>k2)))) {
    int t=i1; i1=i2; i2=t; t=j1; j1=j2; j2=t; t=k1; k1=k2; k2=t;
  }
  index = HASH(i1, j1, k1) + HASH(i2, j2, k2);
  _new = (EDGELIST *) mycalloc(1, sizeof(EDGELIST));
  _new->i1 = i1; _new->j1 = j1; _new->k1 = k1;
  _new->i2 = i2; _new->j2 = j2; _new->k2 = k2;
  _new->vid = vid;
  _new->next = table[index];
  table[index] = _new;
}


/* getedge: return vertex id for edge; return -1 if not set */

int Mc::getedge (EDGELIST **table,
                 int  i1, int j1, int  k1,
                 int  i2, int  j2,int  k2)
{
  EDGELIST *q;
  if (i1>i2 || (i1==i2 && (j1>j2 || (j1==j2 && k1>k2)))) {
    int t=i1; i1=i2; i2=t; t=j1; j1=j2; j2=t; t=k1; k1=k2; k2=t;
  };
  q = table[HASH(i1, j1, k1)+HASH(i2, j2, k2)];
  for (; q != NULL; q = q->next)
    if (q->i1 == i1 && q->j1 == j1 && q->k1 == k1 &&
        q->i2 == i2 && q->j2 == j2 && q->k2 == k2)
      return q->vid;
  return -1;
}


/**** Vertices ****/


/* vertid: return index for vertex on edge:
 * c1->value and c2->value are presumed of different sign
 * return saved index if any; else compute vertex and save */

int Mc::vertid (CORNER* c1, CORNER* c2, PROCESS* p)
{
  VERTEX v;
  POINT a, b;
  int vid = getedge(p->edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k);
  if (vid != -1) return vid;			     /* previously computed */
  a.x = c1->x; a.y = c1->y; a.z = c1->z;
  b.x = c2->x; b.y = c2->y; b.z = c2->z;
  converge(&a, &b, c1->value, &v.position); /* position */
  vnormal(&v.position, p, &v.normal);			   /* normal */
  addtovertices(&p->vertices, v);			   /* save vertex */
  vid = p->vertices.count-1;
  setedge(p->edges, c1->i, c1->j, c1->k, c2->i, c2->j, c2->k, vid);
  return vid;
}


/* addtovertices: add v to sequence of vertices */

void Mc::addtovertices (VERTICES *vertices,VERTEX v)
{
  if (vertices->count == vertices->max) {
    int i;
    VERTEX *_new;
    vertices->max = vertices->count == 0 ? 10 : 2*vertices->count;
    _new = (VERTEX *) mycalloc(vertices->max, sizeof(VERTEX));
    for (i = 0; i < vertices->count; i++) _new[i] = vertices->ptr[i];
    if (vertices->ptr != NULL) free((char *) vertices->ptr);
    vertices->ptr = _new;
  }
  vertices->ptr[vertices->count++] = v;
}

void Mc::vnormal (POINT* point, PROCESS* p, POINT* v)
{
  Vec3f norm, dest(point->x, point->y, point->z);
  
  is->evalNormal(dest, norm);
  v->x = norm[0];
  v->y = norm[1];
  v->z = norm[2];
}


/* converge: from two points of differing sign, converge to zero crossing */

void Mc::converge (POINT* p1, POINT* p2, 
                   double v, 
                   POINT* p)
{
  int i = 0;
  POINT pos, neg;
  if (v < 0) {
    pos.x = p2->x; pos.y = p2->y; pos.z = p2->z;
    neg.x = p1->x; neg.y = p1->y; neg.z = p1->z;
  }
  else {
    pos.x = p1->x; pos.y = p1->y; pos.z = p1->z;
    neg.x = p2->x; neg.y = p2->y; neg.z = p2->z;
  }
  while (1) {
    p->x = 0.5*(pos.x + neg.x);
    p->y = 0.5*(pos.y + neg.y);
    p->z = 0.5*(pos.z + neg.z);
    if (i++ == maxIt) return;
    if ((fun(p->x, p->y, p->z)) > 0.0)
    {pos.x = p->x; pos.y = p->y; pos.z = p->z;}
    else {neg.x = p->x; neg.y = p->y; neg.z = p->z;}
  }
}


double Mc::fun(double x, double y, double z)
{
  return -is->eval(Vec3f(x,y,z));
}

/*Static members*/
const Mc::Corner Mc::corner1[12] = {LBN,LTN,LBN,LBF,RBN,RTN,RBN,RBF,LBN,LBF,LTN,LTF};
const Mc::Corner Mc::corner2[12] = {LBF,LTF,LTN,LTF,RBF,RTF,RTN,RTF,RBN,RBF,RTN,RTF};
const Mc::Direction Mc::leftface[12] = {B,  L,  L,  F,  R,  T,  N,  R,  N,  B,  T,  F};
const Mc::Direction Mc::rightface[12] = {L,  T,  N,  L,  B,  R,  R,  F,  B,  F,  N,  T};

/* History:
* $Log: mc2.cc,v $
* Revision 1.8  2004/04/23 16:17:54  leserpent
* Forgotten to clear indices vector
*
* Revision 1.7  2004/04/06 14:31:32  leserpent
* Removed an useless if in clear()
*
* Revision 1.6  2004/04/03 11:34:02  leserpent
* Added original mc's licence
*
* Revision 1.5  2004/04/03 11:16:00  leserpent
* Added methods: set{InitPoint, CubeSize,	MaxIteration} and enableTet to class Mc
* Added a destructor which free previously allocated vertices.
* Added a getPoints method
*
* Revision 1.4  2004/04/02 08:59:24  leserpent
* Flipped normal
*
* Revision 1.3  2004/04/02 07:25:33  leserpent
* Added a setInitPoint method.
*
* Revision 1.2  2004/04/01 20:57:45  leserpent
* Trivial: Moved Log keyword at the end of the file
*
* Revision 1.1  2004/04/01 20:38:41  leserpent
* Moved private methods from mc2.h to mc2.cc
* Removed useless typedef.
*/
