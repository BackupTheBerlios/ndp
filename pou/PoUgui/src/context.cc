#include "context.h"
#include "opengl.h"

OpenglContext::OpenglContext() {
  m_modelview = new Matrix4d();
  m_projection = new Matrix4d();

  m_fov = 60.0;  
  m_viewaspect = 1024.0/768.0;
}

OpenglContext::~OpenglContext() {
  if( m_modelview )
    delete m_modelview;
  if( m_projection )
    delete m_projection;
}

void OpenglContext::RotateView( int relx, int rely ) {
  m_updatemview = true;
}

void OpenglContext::RotateView( double rx, double ry, double rz ){
  m_updatemview = true;
}

void OpenglContext::ZoomView( int factor ){
  m_updatemview = true;
}

void OpenglContext::ResizeView( int width, int height ){
  m_viewaspect = (double) width / (double) height;
  m_updateproj = true;
}

void OpenglContext::SetFov( double fov ){
  m_fov = fov;
  m_updateproj = true;
}

void OpenglContext::SyncContext() {
  if( m_updateproj ){
    m_updateproj = false;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glLoadMatrixd( (double *)m_projection->data );
    glMatrixMode( GL_MODELVIEW );
  }

  if( m_updatemview ){
    m_updateproj = false;
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glLoadMatrixd( (double *)m_modelview->data );
  }
}
