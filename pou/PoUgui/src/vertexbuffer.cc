/*
 *  VertexBuffer class
 *
 *  Copyright(C) 2004  Dalla Rosa Damien
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Wednesday 31 March 2004:
 *      - glDrawElements fixed thanks to Ben.
 *
 */

#include <string>
#include <iostream>
#include "opengl.h"

int PolyTypes[3] = { GL_POINTS, GL_TRIANGLES, GL_QUADS };

VertexBuffer::VertexBuffer() {
  init();
}

VertexBuffer::VertexBuffer(const std::vector<Point> &vertices, int polytype ) {
  init();
  CreateVertexBuffer(vertices, polytype);
}

VertexBuffer::~VertexBuffer() {
  if( isLocked )
    unLockBuffer();
  if( type == TYPE_VBO )
    ; /* Delete buffer */
}

void VertexBuffer::init() {
  if( !HasVBO() )
    type = TYPE_VERTEXBUFFER;
  else
    type = TYPE_VBO;
  contents = POLY_COORDS;
  isLocked = false;
  mapptr = 0;
}

int VertexBuffer::CreateVertexBuffer( const std::vector<Point> &vertices,
                                      int polytype ) {
  this->polytype = polytype;
  if( type == TYPE_VERTEXBUFFER )
    m_vertices = vertices;
  
  if( type == TYPE_VBO ) {
    
  }
  
  return 0;
}

std::vector<Point> &VertexBuffer::getDataPointer() {
  assert(isLocked);

  if( (type == TYPE_VBO) && !mapptr )
    MapBuffer();

//   if( type == TYPE_VBO )
//     return (Vec3f *)mapptr;

  return m_vertices;
}

void VertexBuffer::Bind() 
{
  glEnableClientState( GL_COLOR_ARRAY );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY );
  if( !m_indices.empty() ){
    printf("Enable indices\n");
    glEnableClientState( GL_INDEX_ARRAY );
  }

  if( type == TYPE_VERTEXBUFFER ){
    glVertexPointer(3, GL_FLOAT, sizeof(Point), &m_vertices[0].pos);
    glColorPointer(3, GL_FLOAT, sizeof(Point), &m_vertices[0].rgb);
    glNormalPointer(GL_FLOAT, sizeof(Point), &m_vertices[0].norm);
  }

}

void VertexBuffer::unBind() 
{
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_INDEX_ARRAY);
}

void VertexBuffer::LockBuffer() 
{
  isLocked = true;
}

void VertexBuffer::unLockBuffer() 
{
  isLocked = false;
  if( (type == TYPE_VBO) && mapptr )
    unMapBuffer();
}

int VertexBuffer::ResizeBuffer( Vec3f *dataptr, int size, int step ) 
{
  return 0;
}

int VertexBuffer::DrawBuffer() 
{
  assert(!isLocked);

  glColor4f( 1.0,1.0,1.0, 0.0 );
  if( m_indices.empty() )
      glDrawArrays( PolyTypes[polytype], 0, m_vertices.size() ); 
  else {
    glDrawElements(PolyTypes[polytype], m_indices.size(), GL_UNSIGNED_INT, 
		   &m_indices[0]);
  }
  return 0;
}

void VertexBuffer::MapBuffer() 
{
  if( type == TYPE_VBO )
    if( !mapptr ){

    }
}

void VertexBuffer::unMapBuffer() 
{
  if( type == TYPE_VBO )
    if( mapptr ){
      
      mapptr = NULL;
    }
}
