#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define TYPE_VERTEXBUFFER 0
#define TYPE_VBO 1

//TODO: ADD SUPPORT FOR: INDEX, VBO

class Vertex3f;

enum{ 
  POLY_POINTS = 0,
    POLY_TRIANGLE,
    POLY_QUAD
    };

class VertexBuffer{
 public:
  VertexBuffer();
  ~VertexBuffer();
  int CreateVertexBuffer( Vertex3f *dataptr, int size, int polytype );
  Vertex3f *GetDataPointer();
  void LockBuffer();
  void unLockBuffer();
  void Bind();
  void unBind();
  int ResizeBuffer( Vertex3f *dataptr, int size );
  int DrawBuffer();
 private:
  void MapBuffer();
  void unMapBuffer();
 private:

  /* Both (vertexbuffer, vbo) */
  int size;
  bool isLocked;
  int polytype;

  /*vertexbuffer*/
  Vertex3f *ptr;

  /* VBO */
  int id;
  int type;
  void *mapptr;
};


#endif
