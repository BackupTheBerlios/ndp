#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define TYPE_VERTEXBUFFER 0
#define TYPE_VBO 1

//TODO: ADD SUPPORT FOR: INDEX, VBO

#include "PointSet.h"
#include "math/vector3.h"

enum{ 
  POLY_POINTS = 0,
    POLY_TRIANGLES=1,
    POLY_QUADS=2
    };

enum{
  POLY_COORDS = 1,
    POLY_NORM = 2,
    POLY_COLORS = 4
};

class VertexBuffer{
 public:
  VertexBuffer();
  VertexBuffer(const std::vector<Point> &vertices, int polytype );
  ~VertexBuffer();

  int CreateVertexBuffer( const std::vector<Point> &vertices, int polytype );
  void init();

  void LockBuffer();
  void unLockBuffer();
  void Bind();
  void unBind();

  int ResizeBuffer( Vec3f *dataptr, int size, int step );
  int DrawBuffer();

  void SetIndices(std::vector<unsigned int> indices) { m_indices = indices; }
  
  std::vector<Point> &VertexBuffer::getDataPointer();
  inline int getSize(){ return m_indices.size(); }
  inline int getPolyType(){ return m_polytype; }
 private:
  void MapBuffer();
  void unMapBuffer();
 private:
  std::vector<unsigned int> m_indices;
  std::vector<Point> m_vertices;

  /* Both (vertexbuffer, vbo) */
  int m_size;
  int m_step;
  bool m_islocked;
  int m_polytype;
  int m_contents;

  /* VBO */
  int m_id;
  int m_type;
  void *m_mapptr;
};


#endif
