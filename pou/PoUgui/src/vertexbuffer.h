#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define TYPE_VERTEXBUFFER 0
#define TYPE_VBO 1

//TODO: ADD SUPPORT FOR: INDEX, VBO

#include "math/vector3.h"

enum{ 
  POLY_POINTS = 0,
    POLY_TRIANGLE,
    POLY_QUAD
    };


enum{
  POLY_COORDS = 1,
    POLY_NORM = 2,
    POLY_COLORS = 4
};

class VertexBuffer{
 public:
  VertexBuffer();
  ~VertexBuffer();
  int CreateVertexBuffer( Vec3f *dataptr, int size, int step, int polytype );
  Vec3f *getDataPointer();
  void LockBuffer();
  void unLockBuffer();
  void Bind();
  void unBind();
  int ResizeBuffer( Vec3f *dataptr, int size, int step );
  int DrawBuffer();
  inline int getSize(){ return size; }
  inline int getStep(){ return step; }
 private:
  void MapBuffer();
  void unMapBuffer();
 private:

  /* Both (vertexbuffer, vbo) */
  int size;
  int step;
  bool isLocked;
  int polytype;
  int contents;

  /*vertexbuffer*/
  Vec3f *ptr;

  /* VBO */
  int id;
  int type;
  void *mapptr;
};


#endif
