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

VertexBuffer::VertexBuffer() 
{
  init();
}

VertexBuffer::VertexBuffer(const std::vector<Point> &vertices, int polytype ) 
{
  init();
  CreateVertexBuffer(vertices, polytype);
}

VertexBuffer::~VertexBuffer() 
{
  if( m_islocked )
    unLockBuffer();
  if( m_type == TYPE_VBO )
    ; /* Delete buffer */
}

void 
VertexBuffer::init() 
{
  if( !HasVBO() )
    m_type = TYPE_VERTEXBUFFER;
  else
    m_type = TYPE_VBO;
  m_islocked = false;
  m_mapptr = 0;
}

int 
VertexBuffer::CreateVertexBuffer( const std::vector<Point> &vertices,
				  int polytype ) 
{
  m_polytype = polytype;
  if( m_type == TYPE_VERTEXBUFFER )
    m_vertices = vertices;
  
  if( m_type == TYPE_VBO ) 
    {
    }
  
  return 0;
}

std::vector<Point> &
VertexBuffer::getDataPointer() 
{
  assert(m_islocked);

  if( (m_type == TYPE_VBO) && !m_mapptr )
    MapBuffer();

  //if( m_type == TYPE_VBO )
  //return (Vec3f *)m_mapptr;

  return m_vertices;
}

void 
VertexBuffer::Bind() 
{
  glEnableClientState( GL_COLOR_ARRAY );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY );

  if( m_type == TYPE_VERTEXBUFFER )
    {
      glVertexPointer(3, GL_FLOAT, sizeof(Point), &m_vertices[0].pos);
      glColorPointer(3, GL_FLOAT, sizeof(Point), &m_vertices[0].rgb);
      glNormalPointer(GL_FLOAT, sizeof(Point), &m_vertices[0].norm);
    }
}

void 
VertexBuffer::unBind() 
{
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void 
VertexBuffer::LockBuffer() 
{
  m_islocked = true;
}

void 
VertexBuffer::unLockBuffer() 
{
  m_islocked = false;
  if( (m_type == TYPE_VBO) && m_mapptr )
    unMapBuffer();
}

int 
VertexBuffer::ResizeBuffer( Vec3f *dataptr, int size, int step ) 
{
  return 0;
}

int 
VertexBuffer::DrawBuffer() 
{
  assert(!m_islocked);

  glColor4f( 1.0,1.0,1.0, 0.0 );
  if( m_indices.empty() )
    glDrawArrays( PolyTypes[m_polytype], 0, m_vertices.size() ); 
  else {
    glDrawElements(PolyTypes[m_polytype], m_indices.size(), GL_UNSIGNED_INT, 
		   &m_indices[0]);
  }
  return 0;
}

void 
VertexBuffer::MapBuffer() 
{
  if( m_type == TYPE_VBO )
    if( !m_mapptr ){

    }
}

void 
VertexBuffer::unMapBuffer() 
{
  if( m_type == TYPE_VBO )
    if( m_mapptr )
      {
	m_mapptr = NULL;
      }
}
