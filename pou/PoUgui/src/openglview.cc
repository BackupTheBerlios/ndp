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
 * $Log: openglview.cc,v $
 * Revision 1.32  2004/04/21 11:40:56  ob821
 * depth  test for points rendering
 *
 * Revision 1.31  2004/04/07 18:23:17  ob821
 * segfault fix
 * lighting bugfix
 *
 * Revision 1.30  2004/04/06 20:00:27  leserpent
 * Use killTimer(timerid) instead of killTimers()(see qt doc)
 *
 * Revision 1.29  2004/04/06 16:18:45  leserpent
 * Added log keyword
 *
 * Revision 1.28  2004/04/06 14:26:09  leserpent
 * Changed two glClear into one
 * Changed a paintGL to updateGL
 *
 *  Tuesday 30 March 2004:
 *      - Use VertexBuffer::DrawBuffer() for rendering
 *
 *  Sunday 28 March 2004:
 *      - Motion support (Dalla Rosa Damien )
 */

#include <iostream>

#include "opengl.h"
#include "openglview.h"
#include "mwindow.h"
#include "math/vector3.h"

bool isOpenglReady = false;

#define FRAME_DELAY 1 //MAX = 1000 fps

/* -+-+-+-+-+-+-+-+-+ OpenglWidget +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglWidget::OpenglWidget( QWidget *parent, const char *name, 
			    VertexBuffer *vbuffer ) 
  :QGLWidget( parent, name ), m_timerid(-1)
{
  m_vbuffer = vbuffer;
  m_glcontext = new OpenglContext( this );
  m_idledraw = false;

  m_glcontext->ShowFps( false );
  m_glcontext->ShowStats( false );
  m_glcontext->ShowLightPosition( false );
}

OpenglWidget::~OpenglWidget() 
{
  if(m_timerid != -1)
    killTimer(m_timerid);
  delete m_glcontext;
}

void 
OpenglWidget::SetIdleDraw( bool state )
{
  if( state == m_idledraw )
    return;

  m_idledraw = state;

  if( m_idledraw )
    m_timerid = startTimer( FRAME_DELAY );
  else
    if(m_timerid != -1)
      killTimer(m_timerid);
}

void 
OpenglWidget::initializeGL() 
{
  //Initialise OpenGL si c'est la première utilisation
  if( !isOpenglReady )
    OpenglInit();
  isOpenglReady = true;
   
  clearGL();
  glDisable (GL_BLEND);
  m_glcontext->SetDepthTest (false);
  m_vbuffer->Bind();
  /* Activate some flags for poly rendering */
  if( m_vbuffer->getPolyType () == POLY_TRIANGLES ){
    /* Don't cull points only polys */
    glEnable (GL_CULL_FACE);
    glCullFace (GL_FRONT);
    m_glcontext->SetDepthTest (true);
    /* Enable Lighting*/
    SetLighting (true, LIGHT_SMOOTH);
  }
  /* Enable zbuffer */
  m_glcontext->SetDepthTest (true);
      
  m_glcontext->OppositeColorFlags();
}

void 
OpenglWidget::clearGL() 
{
  qglClearColor (black);
}

void 
OpenglWidget::resizeGL( int w, int h ) 
{
  glViewport (0, 0, (GLint)w, (GLint)h);

  m_glcontext->SetClipDistance (0.001, 1500.0);
  m_glcontext->SetFov (60.0);
  m_glcontext->SetViewSize (w, h);
  m_glcontext->SyncContext();
  if( !m_idledraw )
    updateGL();
}

void 
OpenglWidget::paintGL() 
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  m_glcontext -> SyncContext();
  
  if( m_vbuffer )
    m_vbuffer -> DrawBuffer();

  m_glcontext -> DrawHud();
  
  swapBuffers();
}

void 
OpenglWidget::mousePressEvent( QMouseEvent *e ) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    m_glcontext -> StartRotationMode( e->x(), e->y() );
    break;
  case QMouseEvent::RightButton: // Reset transformation
    m_glcontext -> InitRotationMode();
    if( !m_idledraw )
      updateGL();
    break;
  default:
    break;
  }    
  m_glcontext -> SyncContext();
}

void 
OpenglWidget::ParseKey( int key, int key_ascii )
{
  if( tolower(key_ascii) == 'f' )
    {
      bool flag = !m_glcontext->getFpsState ();
      SetIdleDraw (flag);
      m_glcontext->ShowFps (flag);
    }
  if( tolower(key_ascii) == 's' )
    m_glcontext->ShowStats (!(m_glcontext->getStatsState ()));

  if( tolower(key_ascii) == 'p' )
    m_glcontext->ShowLightPosition (!(m_glcontext->getLightPositionState ()));

  if( tolower(key_ascii) == 'c' )
    m_glcontext->OppositeColorFlags ();

  if( tolower(key_ascii) == 'q' )
    m_glcontext->OppositePolygonMode ();

  if( key == Qt::Key_Left)
   m_glcontext->MoveLight (0, 5, 0.0);

  if(  key == Qt::Key_Right )
    m_glcontext->MoveLight (0, -5, 0.0);

  if(  key == Qt::Key_Down )
    m_glcontext->MoveLight (5, 0, 0.0);

  if(  key == Qt::Key_Up )
    m_glcontext->MoveLight (-5, 0, 0.0);

  if( tolower(key_ascii) == '+' )
    m_glcontext->MoveLight (0, 0, -0.2);

  if( tolower(key_ascii) == '-' )
    m_glcontext->MoveLight (0, 0, 0.2);

  if( key_ascii == '1' )
    m_glcontext->SetLightType (LIGHT_FLAT);

  if( key_ascii == '2' )
    m_glcontext->SetLightType (LIGHT_SMOOTH);

  if( key_ascii == '3' )
    m_glcontext->SetLightType (LIGHT_PIXEL);

  if( !m_idledraw )
    updateGL();
}

void 
OpenglWidget::mouseReleaseEvent( QMouseEvent * e) 
{
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    m_glcontext->StopRotationMode();
    m_glcontext->SyncContext();
    if( !m_idledraw )
      updateGL();
    break;
  default:
    break;
  }    
}

void 
OpenglWidget::mouseMoveEvent( QMouseEvent *e ) 
{
  if(e->state()&QMouseEvent::LeftButton) {
    m_glcontext->RotateView (e->x(), e->y());
    m_glcontext->SyncContext();
    if( !m_idledraw )
      updateGL();
  }
}

void 
OpenglWidget::wheelEvent ( QWheelEvent * e ) 
{
  e->accept();
  m_glcontext->ZoomView (e->delta());
  m_glcontext->SyncContext();
  if( !m_idledraw )
    updateGL();
}


void OpenglWidget::SetLighting( bool state, int type )
{
  m_glcontext->SetLightType (type);
  m_glcontext->SetLighting ( state );
  m_glcontext->OppositeColorFlags();
  m_glcontext->OppositePolygonMode();
  m_glcontext->MoveLight (0, 0, 1.5);
}

void OpenglWidget::timerEvent( QTimerEvent *e) 
{
  updateGL();
}

/* -+-+-+-+-+-+-+-+-+ OpenglView +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglView::OpenglView( QWorkspace *parent, VertexBuffer *vbuffer ) 
  : QMainWindow( parent, "OpenGL View", 0 )
{
  m_glwidget = new OpenglWidget (this, "Rendering Widget", vbuffer);
  setCaption ("OPENGL View");
  // Permet de propager les évènements
  setCentralWidget (m_glwidget);
}

OpenglView::~OpenglView() 
{
  delete m_glwidget;
}

void 
OpenglView::closeEvent( QCloseEvent *e )
{
  delete m_glwidget;
  m_glwidget = NULL;
  hide();
}

void 
OpenglView::keyPressEvent( QKeyEvent *e )
{
  m_glwidget->ParseKey( e->key(), e->ascii() );
}
