#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "PointSet.h"
#include "math/vector3.h"

class VertexBuffer {
 public:
  enum PolyType {
    POLY_POINTS = 0,
    POLY_TRIANGLES,
    POLY_QUADS
  };

 public:
  VertexBuffer();
  VertexBuffer( const std::vector<Point> &vertices, PolyType polytype );
  ~VertexBuffer();

  int CreateVertexBuffer( const std::vector<Point> &vertices, PolyType polytype );
  void init();

  void LockBuffer();
  void unLockBuffer();
  void Bind();
  void unBind();

  int DrawBuffer();

  void SetIndices( std::vector<unsigned int> indices ) { m_indices = indices; }
  
  std::vector<Point> &VertexBuffer::getDataPointer();
  inline int getSize() { return m_indices.size(); }
  inline int getPolyType() { return m_polytype; }

 private:
  std::vector<unsigned int> m_indices;
  std::vector<Point> m_vertices;

  bool m_islocked;
  int m_polytype;
};


#endif
