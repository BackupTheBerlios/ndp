/*
 *  OpenglWidget and OpenglView class
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
 *  Sunday 28 March 2004:
 *      - Motion support (Dalla Rosa Damien )
 */

#include <iostream>

#include "libgui.h"
#include "opengl.h"
#include "math/vector3.h"

bool isOpenglReady = false;

/* -+-+-+-+-+-+-+-+-+ OpenglWidget +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglWidget::OpenglWidget( QWidget *parent, const char *name, 
			    VertexBuffer *vbuffer ) 
  :QGLWidget( parent, name )
{
  vb = vbuffer;
  glcontext = new OpenglContext();
}

OpenglWidget::~OpenglWidget() {
  if( glcontext )
    delete glcontext;
}

void OpenglWidget::initializeGL() 
{
  //Initialise OpenGL si c'est la première utilisation
  if( !isOpenglReady )
    OpenGL_Init();
  isOpenglReady = true;
   
  clearGL();
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( 0.0, 0.0, 3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

void OpenglWidget::clearGL() 
{
  qglClearColor( black );
}

void OpenglWidget::resizeGL( int w, int h ) 
{
  glViewport( 0, 0, (GLint)w, (GLint)h );

  glcontext -> SetClipDistance( 0.001, 1500.0 );
  glcontext -> SetFov( 60.0 );
  glcontext -> SetViewSize( w, h );
  glcontext -> SyncContext();

  paintGL();
}

void OpenglWidget::paintGL() {
  //float m[4][4];

  glClear( GL_COLOR_BUFFER_BIT );
  
  //glPushMatrix();
  glcontext -> SyncContext();
  if( vb ) {
    qglColor( white );

    glBegin( GL_POINTS );
    vb->LockBuffer();
    Vec3f *ptr = (Vec3f *)vb->getDataPointer();
    
    for( int i=0; i< vb -> getSize()*vb -> getStep(); i+= vb -> getStep() ) {
      glColor3f( ptr[i+2].x, ptr[i+2].y, ptr[i+2].z);
      glVertex3f( ptr[i].x, ptr[i].y, ptr[i].z );
    }
    vb->unLockBuffer();
    glEnd();
  }
  //glPopMatrix();
  swapBuffers();
}

void OpenglWidget::mousePressEvent( QMouseEvent *e ) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    glcontext -> StartRotationMode( e->x(), e->y() );
    break;
  case QMouseEvent::RightButton: // Reset transformation
    glcontext -> InitRotationMode();
    updateGL();
    break;
  default:
    break;
  }    
  glcontext -> SyncContext();
}

void OpenglWidget::mouseReleaseEvent( QMouseEvent * e) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    glcontext->StopRotationMode();
    glcontext->SyncContext();
    updateGL();
    break;
  default:
    break;
  }    
}

void OpenglWidget::mouseMoveEvent( QMouseEvent *e ) {
  if(e->state()&QMouseEvent::LeftButton) {
    glcontext->RotateView( e->x(), e->y() );
    glcontext->SyncContext();
    updateGL();
  }
}

void OpenglWidget::wheelEvent ( QWheelEvent * e ) {
  e->accept();
  glcontext -> ZoomView( e->delta() );
  glcontext -> SyncContext();
  updateGL();
}

/* -+-+-+-+-+-+-+-+-+ OpenglView +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglView::OpenglView( QWorkspace *parent, VertexBuffer *vbuffer ) 
  : QMainWindow( parent, "OpenGL View", 0 )
{
  glwidget = new OpenglWidget( this, "Rendering Widget", vbuffer );
  setCaption( "OPENGL View" );
  // Permet de propager les évènements
  setCentralWidget( glwidget );
}

OpenglView::~OpenglView() 
{
  if( glwidget )
    delete glwidget;
}

