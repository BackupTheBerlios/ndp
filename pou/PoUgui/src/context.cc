/*
 *  Opengl Context manager
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
 *      - Rotation functions added ( Benjamin Grange )
 *      - Zoom functions added ( Benjamin Grange )
 *      - m_width and m_height added ( Damien Dalla Rosa )
 */


#include <cmath>
#include <qdatetime.h>
#include "context.h"
#include "openglview.h"
#include "opengl.h"
#include "vertexbuffer.h"

#define DEG2RAD( x ) (x) * M_PI / 180.0

const float OpenglContext::DEF_ZOOM;
const float OpenglContext::INVSQRT2 = 1.0f/std::sqrt(2.0f);

OpenglContext::OpenglContext( OpenglWidget *parent ) 
  : m_zoomfactor(DEF_ZOOM) 
{
  m_modelview.Identity();
  m_projection.Identity();
  m_fov = 60.0;  
  m_width = 1024;
  m_height = 768;
  m_viewaspect = 1024.0/768.0;
  m_updatemview = true;  
  m_updateproj = true;
  m_lasttime = -1;
  m_showfps = false;
  m_parent = parent;
  m_lightrx = 0;
  m_lightry = 0;
  m_lightdistance = 0;
  m_lightpos = Vec3f( 0.0f, 0.0f, 0.0f );
  /* init font */
  m_font.setStyleStrategy( QFont::OpenGLCompatible );
  m_font.setFamily("fixed");
  m_font.setRawMode(true);
  m_font.setPixelSize(10);           // Workaround for a bug with renderText()
  m_font.setFixedPitch ( true )  ;   // and nvidia's driver
  m_font.setStyleHint(QFont::AnyStyle, QFont::PreferBitmap);
}

OpenglContext::~OpenglContext() 
{ }

//Find Z so that point v(mousex,mousey,Z) is on a unit sphere.
//if we are out of the sphere, we find Z so that point v is on a hyperbole.
//The hyperbole allow continuity when we go away from the sphere.
//X=1/sqrt(2) is the x-coordinate of the intersection between a unit circle and
//the hyperbole 1/2x
//Reference: Terence J. Grant nehe.gamedev.net and nvidia's
//trackball.h(Gavin Bell)
void OpenglContext::mapToSphere(Vec3f &v) 
{
  float len2;
      
  v.x = (2*v.x)/float(m_width-1)-1;
  v.y = 1-(2*v.y)/float(m_height-1);
  v.z = 0;
  if((len2 = v.length2())<INVSQRT2)
      v.z = std::sqrt(1.0-len2); // We are on the sphere
  else 
      v.z = 1.0f/(2*std::sqrt(len2)); // On the hyperbole
}

void OpenglContext::StartRotationMode( int x, int y, bool camera )
{
  if( camera ){
    m_startVector.setValues(x, y, 0);
    mapToSphere(m_startVector);
    m_updatemview = true;
  }
  else{

  }
}

void OpenglContext::StopRotationMode( bool camera )
{
  if( camera ){
    m_startOrientation = m_orientation;
    m_updatemview = true;
  }
  else {

  }
}

void OpenglContext::InitRotationMode( bool camera )
{
  if( camera ){
    m_orientation.toIdentity();
    m_startOrientation.toIdentity();
    m_zoomfactor = DEF_ZOOM;
    m_updatemview = true;
  }
  else {

  }
}

void OpenglContext::RotateView( int x, int y, bool camera ) 
{
  if( camera ) {
    Quaternionf q;
    Vec3f endVector(x, y, 0);
    mapToSphere(endVector);
    q.toRotationArc(m_startVector, endVector);
    m_orientation = q*m_startOrientation;
    m_orientation.normalize();
    m_updatemview = true;
  }
  else {

  }
}

void OpenglContext::ZoomView( double factor, bool camera )
{
  if( camera ){
    if( factor < 0 )
      m_zoomfactor *= 0.9;
    else
      m_zoomfactor *= 1.1;
    
    m_updatemview = true;
  }
}

void OpenglContext::SetViewSize( int width, int height )
{
  m_viewaspect = (double) width / (double) height;
  m_width = width;
  m_height = height;
  m_updateproj = true;
}

void OpenglContext::SetFov( double fov )
{
  m_fov = fov;
  m_updateproj = true;
}

void OpenglContext::SetClipDistance( double near, double far ) 
{
  m_near = near;
  m_far = far;
  m_updateproj = true;
}

void OpenglContext::SetDepthTest( bool state ){
  m_depthtest = state;
  if( !state )
    glDisable( GL_DEPTH_TEST );
  else{
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );   
    glEnable( GL_DEPTH_TEST );
  }
}

void OpenglContext::SetLighting( bool state )
{
  m_lightstate = state;
  if (m_lightstate)
    {
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      glEnable (GL_LIGHTING);
      glEnable (GL_LIGHT0);
      float pos[4] = { m_lightpos.x , m_lightpos.y, m_lightpos.z, 1.0f };
      glLightfv (GL_LIGHT0,GL_POSITION,(float *)pos);
    }
}

void OpenglContext::SetLightType( int type )
{
  m_lighttype = type;
}

void OpenglContext::ShowLightPosition( bool flag )
{
  m_lightdraw = flag;
}

void OpenglContext::MoveLight( int anglex, int angley, double distance ){
  double cTheta, sTheta, cPhi, sPhi, theta, phi;
  m_lightdistance += distance;
  m_lightrx += anglex;
  //m_lightrx %= 360; // [0, PI]
  m_lightry += angley;
  //m_lightry %= 360; // [0, 2PI ]
  
  theta = DEG2RAD((double)m_lightry );
  phi = DEG2RAD((double)m_lightrx );

  cTheta = cos( theta );
  cPhi = cos( phi );
  sTheta = sin( theta );
  sPhi = sin( phi );

  m_lightpos.x = m_lightdistance * cTheta * sPhi;
  m_lightpos.z = m_lightdistance * sTheta * sPhi;
  m_lightpos.y = m_lightdistance * cPhi;
  float pos[4] = { m_lightpos.x , m_lightpos.y, m_lightpos.z, 1.0f };
  glLightfv (GL_LIGHT0,GL_POSITION,(float *)pos);
}

void OpenglContext::DrawHud()
{
  glPushAttrib( GL_ENABLE_BIT );
  glDisable (GL_LIGHTING);
  
  /* Draw the light position using 2 quads */
  if( m_lightdraw ){
    glDisable( GL_CULL_FACE );
    glColor3f( 0.0, 1.0, 0.0 );
    glBegin( GL_QUADS );
    glVertex3f( m_lightpos.x - 0.1, m_lightpos.y - 0.1, m_lightpos.z);
    glVertex3f( m_lightpos.x + 0.1, m_lightpos.y - 0.1, m_lightpos.z);
    glVertex3f( m_lightpos.x + 0.1, m_lightpos.y + 0.1, m_lightpos.z);
    glVertex3f( m_lightpos.x - 0.1, m_lightpos.y + 0.1, m_lightpos.z);

    glVertex3f( m_lightpos.x - 0.1, m_lightpos.y, m_lightpos.z-0.1);
    glVertex3f( m_lightpos.x + 0.1, m_lightpos.y, m_lightpos.z-0.1);
    glVertex3f( m_lightpos.x + 0.1, m_lightpos.y, m_lightpos.z+0.1);
    glVertex3f( m_lightpos.x - 0.1, m_lightpos.y, m_lightpos.z+0.1);

    glEnd();
    glColor3f( 1.0, 1.0, 1.0 );
  }

  /* Disable zbuffer for text rendering */
  glDisable(GL_DEPTH_TEST);
  
  if( m_showstats ){
    QString spolys("Triangles ");
    VertexBuffer *vb = m_parent ->getVertexBuffer();
    spolys += QString::number(vb->getSize());
    glColor3f( 1.0, 1.0, 1.0 );
    m_parent -> renderText( 10, 40, spolys, m_font );
  }

  if( m_showfps ){
    int curtime;
    QString sfps("FPS: ");
    QTime t = QTime::currentTime();
    curtime = ( t.hour()*3600 + t.minute()*60 + t.second())*1000 + t.msec();
    m_frames ++;
    
    if( m_lasttime == -1 ) {
      m_lasttime = curtime;
      m_frames = 0;
      m_fps = 0.0f;
    }
    
    if( (curtime - m_lasttime) > 500 ){
      m_fps = 1000.0 * (float)m_frames /(curtime - m_lasttime);
      m_frames = 0;
      m_lasttime = curtime;
    }
    
    sfps += QString::number(m_fps);
    glColor3f( 1.0, 1.0, 1.0 );
    m_parent -> renderText( 10, 20, sfps, m_font );
  }
  /* Restore gl flags */
  glPopAttrib();
  
}

void OpenglContext::ShowFps( bool flag )
{
  m_showfps = flag;
  if( !m_showfps )
    m_lasttime = -1;
}

void OpenglContext::ShowStats( bool flag )
{
  m_showstats = flag;
}

void OpenglContext::SyncContext() 
{
  if( m_updateproj ){
    m_updateproj = false;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    /* Build the Matrix */
    double tfov = tan( DEG2RAD( m_fov / 2.0 ) );
    double top = m_near * tfov;
    double right = top * m_viewaspect;

    m_projection.data[0][0] = m_near / right ;
    m_projection.data[1][1] = m_near / top ;
    m_projection.data[2][2] = -( m_far + m_near ) / ( m_far - m_near ) ;
    m_projection.data[2][3] = -1.0 ;
    m_projection.data[3][2] = ( -2 * m_far * m_near ) / ( m_far - m_near ) ;

    /*Send the Matrix*/
    glLoadMatrixd( (double *)m_projection.data );
    glMatrixMode( GL_MODELVIEW );
  }

  if( m_updatemview ){
    m_updateproj = false;
    glMatrixMode( GL_MODELVIEW );
    /* Build the Matrix */
    m_orientation.unitToMatrix44( m_modelview.data );
    /* Send the Matrix */
    glLoadMatrixf( (float *)m_modelview.data );
    /* Apply Zoom */
    glScalef( m_zoomfactor, m_zoomfactor, m_zoomfactor );
  }
}
