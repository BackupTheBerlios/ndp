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

  void LockBuffer();
  void unLockBuffer();
  void Bind();
  void unBind();

  int DrawBuffer();

  void SetIndices( std::vector<unsigned int> indices ) { m_indices = indices; }
  
  std::vector<Point> &getDataPointer();

  inline int getSize() { return m_indices.size(); }
  inline int getPolyType() { return m_polytype; }

 private:
  void Init();

  std::vector<unsigned int> m_indices;
  std::vector<Point> m_vertices;

  bool m_islocked;
  int m_polytype;
};


#endif
