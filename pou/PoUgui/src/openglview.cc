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
 *  Tuesday 30 March 2004:
 *      - Use VertexBuffer::DrawBuffer() for rendering
 *
 *  Sunday 28 March 2004:
 *      - Motion support (Dalla Rosa Damien )
 */

#include <iostream>

#include "libgui.h"
#include "opengl.h"
#include "math/vector3.h"

bool isOpenglReady = false;

#define FRAME_DELAY 1 //MAX = 1000 fps

/* -+-+-+-+-+-+-+-+-+ OpenglWidget +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglWidget::OpenglWidget( QWidget *parent, const char *name, 
			    VertexBuffer *vbuffer ) 
  :QGLWidget( parent, name )
{
  vb = vbuffer;
  glcontext = new OpenglContext( this );
  m_idledraw = false;
  /* Polygon window => show fps */
  if( vb -> getPolyType() == POLY_TRIANGLES ){
    m_idledraw = true;
    glcontext -> ShowFps( false );
    glcontext -> ShowStats( false );
  }
  
  //setFocusProxy( this );
}

OpenglWidget::~OpenglWidget() {
  killTimers();
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
  glEnable( GL_CULL_FACE );
  glEnable( GL_DEPTH );
  glCullFace( GL_BACK );
  vb -> Bind();
  if( m_idledraw )
    startTimer( FRAME_DELAY );
  SetLighting (true,GL_DIFFUSE,1.0,1.0,1.0);

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
  if( !m_idledraw )
    paintGL();
}

void OpenglWidget::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT );
  glClear( GL_DEPTH_BUFFER_BIT );
  
  glcontext -> SyncContext();

  if( vb )
    vb -> DrawBuffer();

  glcontext -> DrawHud();
  
  swapBuffers();
}

void OpenglWidget::mousePressEvent( QMouseEvent *e ) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    glcontext -> StartRotationMode( e->x(), e->y(), true );
    break;
  case QMouseEvent::RightButton: // Reset transformation
    glcontext -> InitRotationMode( true );
    if( !m_idledraw )
      updateGL();
    break;
  default:
    break;
  }    
  glcontext -> SyncContext();
}

void OpenglWidget::ParseKey( int key )
{
  if( tolower(key) == 'f' )
    glcontext -> ShowFps( !(glcontext -> getFpsState()) );
  if( tolower(key) == 's' )
    glcontext -> ShowStats( !(glcontext -> getStatsState()) );
}

void OpenglWidget::mouseReleaseEvent( QMouseEvent * e) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    glcontext->StopRotationMode( true );
    glcontext->SyncContext();
    if( !m_idledraw )
      updateGL();
    break;
  default:
    break;
  }    
}

void OpenglWidget::mouseMoveEvent( QMouseEvent *e ) {
  if(e->state()&QMouseEvent::LeftButton) {
    glcontext->RotateView( e->x(), e->y(), true );
    glcontext->SyncContext();
    if( !m_idledraw )
      updateGL();
  }
}

void OpenglWidget::wheelEvent ( QWheelEvent * e ) {
  e->accept();
  glcontext -> ZoomView( e->delta(), true );
  glcontext -> SyncContext();
  if( !m_idledraw )
    updateGL();
}


void OpenglWidget::SetLighting( bool state, int type, float x, float y, 
				float z ){
  glcontext -> SetLightType (type);
  glcontext -> SetLightPosition (x, y, z);
  glcontext -> SetLighting ( state );
  glcontext -> DrawLightPosition (false);
}

void OpenglWidget::timerEvent( QTimerEvent *e) {
  //printf("DRAW\n");
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
  delete glwidget;
}

void OpenglView::closeEvent( QCloseEvent *e )
{
  delete glwidget;
  glwidget = NULL;
  hide();
}

void OpenglView::keyPressEvent( QKeyEvent *e )
{
  glwidget->ParseKey( e->ascii() );
}
