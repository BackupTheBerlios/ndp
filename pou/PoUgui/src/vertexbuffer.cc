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
 */

#include <string>
#include <iostream>
#include "opengl.h"

int PolyTypes[3] = { GL_POINTS, GL_TRIANGLES, GL_QUADS };

VertexBuffer::VertexBuffer() {
  if( !HasVBO() )
    type = TYPE_VERTEXBUFFER;
  else
    type = TYPE_VBO;
  contents = POLY_COORDS;
  isLocked = false;
  ptr = NULL;
  mapptr = NULL;
  m_indices = NULL;
}

VertexBuffer::~VertexBuffer() {
  if( isLocked )
    unLockBuffer();
  if( type == TYPE_VBO )
    ; /* Delete buffer */
  else
    if( ptr )
      delete [] ptr;

  delete[] m_indices;
}

// size = number of points 
// step = Vec3f per point
// step * size = # of Vec3f in the buffer
int VertexBuffer::CreateVertexBuffer( Vec3f *dataptr, int size, int step, 
				      int polytype) {
  if( type == TYPE_VERTEXBUFFER ) {
    if( ptr )
      delete [] ptr;
    this->size = size;
    this->step = step;
    this->polytype = polytype;

    ptr = new Vec3f [ size*step ];
    if( !ptr )
      return -1;
    memcpy( ptr, dataptr, size*step*sizeof(Vec3f) );
  }
  
  if( type == TYPE_VBO ) {
    
  }
  this->contents = contents;

  return 0;
}

Vec3f *VertexBuffer::getDataPointer() {
  if( !isLocked )
    return NULL;

  if( (type == TYPE_VBO) && !mapptr )
    MapBuffer();

  if( type == TYPE_VBO )
    return (Vec3f *)mapptr;

  return ptr;
}

void VertexBuffer::Bind() 
{
  glEnableClientState( GL_COLOR_ARRAY );
  glEnableClientState( GL_VERTEX_ARRAY );
  glEnableClientState( GL_NORMAL_ARRAY );
  if( m_indices ){
    printf("Enable indices\n");
    glEnableClientState( GL_INDEX_ARRAY );
  }

  if( type == TYPE_VERTEXBUFFER ){
    glVertexPointer( 3, GL_FLOAT, step*sizeof(Vec3f), ptr );
    glColorPointer( 3, GL_FLOAT, step*sizeof(Vec3f), ptr + 2 );
    glNormalPointer( GL_FLOAT, step*sizeof(Vec3f), ptr + 1 );
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
  int n = m_indices?m_indices_count:size;
  /* Don't draw locked buffers */
  if( isLocked )
    return -1;
  glColor3f( 1.0,1.0,1.0 );
  if( !m_indices )
      glDrawArrays( PolyTypes[polytype], 0, n ); 
  else {
   glDrawElements(PolyTypes[polytype], m_indices_count, GL_UNSIGNED_INT, m_indices);
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

void VertexBuffer::SetIndices( void *data, int size )
{
  delete[] m_indices;
  m_indices = NULL;

  if( !data )
    return ;
  
  m_indices = new unsigned int [size];
  m_indices_count = size;
  memcpy( m_indices, data, size*sizeof( int ) );
}

