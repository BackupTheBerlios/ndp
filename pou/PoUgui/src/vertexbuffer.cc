#include <string>
#include <iostream>
#include "opengl.h"
//TODO: virer include que pour Vertex3f
#include "ims.h"

VertexBuffer::VertexBuffer() {
  if( !HasVBO() )
    type = TYPE_VERTEXBUFFER;
  else
    type = TYPE_VBO;
  std::cout<<"New Buffer: type = "<< type <<"\n";
  isLocked = false;
  ptr = NULL;
  mapptr = NULL;
}

VertexBuffer::~VertexBuffer() {
  if( isLocked )
    unLockBuffer();
  if( type == TYPE_VBO )
    ; /* Delete buffer */
  else
    if( ptr )
      delete [] ptr;
}


int VertexBuffer::CreateVertexBuffer( Vertex3f *dataptr, int size, 
				      int polytype ) {
  if( type == TYPE_VERTEXBUFFER ) {
    if( ptr )
      delete [] ptr;
    this->size = size;
    this->polytype = polytype;
    ptr = new Vertex3f [ size ];
    if( !ptr )
      return -1;
    memcpy( ptr, dataptr, size*sizeof(Vertex3f) );
  }
  
  if( type == TYPE_VBO ) {
    
  }

  return 0;
}

Vertex3f *VertexBuffer::GetDataPointer() {
  if( !isLocked )
    return NULL;

  if( (type == TYPE_VBO) && !mapptr )
    MapBuffer();

  if( type == TYPE_VBO )
    return (Vertex3f *)mapptr;

  return ptr;
}

void VertexBuffer::LockBuffer() {
  isLocked = true;
}

void VertexBuffer::unLockBuffer() {
  isLocked = false;
  if( (type == TYPE_VBO) && mapptr )
    unMapBuffer();
}

int VertexBuffer::ResizeBuffer( Vertex3f *dataptr, int size ) {
  return 0;
}

int VertexBuffer::DrawBuffer() {
  /* Don't draw locked buffers */
  if( isLocked )
    return -1;
  return 0;
}

void VertexBuffer::MapBuffer() {
  if( type == TYPE_VBO )
    if( !mapptr ){

    }
}

void VertexBuffer::unMapBuffer() {
  if( type == TYPE_VBO )
    if( mapptr ){
      
      mapptr = NULL;
    }
}
