/*
 *  Opengl Context manager
 *
 *  Copyright(C) 2004  Damien Dalla Rosa, Thomas Arcila,
 *                      Loic Martin, Benjamin Grange
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


#include <cmath>
#include <qdatetime.h>
#include "context.h"
#include "openglview.h"
#include "opengl.h"
#include "vertexbuffer.h"
#include "utils.h"

//Static members initialisation
const float OpenglContext::DEF_ZOOM;

//gcc 3.0 wants to have the size of the array
OpenglContext::helpStruct OpenglContext::helpInfos[10] = {
  'h', "Hide/Unhide this help screen",
  'p', "Hide/Unhide light",
  's', "Hide/Unhide statistics",
  'f', "Hide/Unhide fps",
  'c', "Enable/Disable colors",
  'q', "Enable/Disable wireframe",
  '+', "Increase light distance from center",
  '-', "Decrease light distance from center",
  '1', "Enable flat shading",
  '2', "Enable gouraud shading"
};

OpenglContext::OpenglContext (OpenglWidget *parent) 
  : m_zoomfactor(DEF_ZOOM), m_light(0)
{
  m_modelview.Identity();
  m_fov = 60.0;  
  m_width = 1024;
  m_height = 768;
  m_viewaspect = 1024.0/768.0;
  m_near = 0.001;
  m_far = 1000.0;
  m_updatemview = true;  
  m_updateproj = true;
  m_lasttime = -1;
  m_showfps = false;
  m_showhelp = false;
  m_parent = parent;

  m_polygonmode = false;
  m_colorflag = true;
  /* init font */
  m_font.setFamily("fixed");
  m_font.setRawMode(true);
  m_font.setPixelSize(10);           // Workaround for a bug with renderText()
  m_font.setFixedPitch ( true )  ;   // and nvidia's driver
  m_font.setStyleHint(QFont::AnyStyle, QFont::PreferBitmap);

  m_tball = new Trackball(m_width, m_height);
}

OpenglContext::~OpenglContext () 
{
  delete m_tball;
}

void 
OpenglContext::StartRotationMode (int x, int y)
{
  m_tball->startRotation(x, y);
}

void 
OpenglContext::StopRotationMode ()
{
  m_tball->stopRotation();
  m_updatemview = true;
}

void 
OpenglContext::InitRotationMode ()
{
  m_zoomfactor = DEF_ZOOM;
  m_updatemview = true;
}

void 
OpenglContext::RotateView (int x, int y) 
{
  m_tball->computeOrientation(x, y);
  m_updatemview = true;
}

void 
OpenglContext::ZoomView (double factor)
{
  if (factor < 0)
    m_zoomfactor *= 0.9;
  else
    m_zoomfactor *= 1.1;
  
  m_updatemview = true;
}

void 
OpenglContext::SetViewSize (int width, int height)
{
  m_viewaspect = (double) width / (double) height;
  m_width = width;
  m_height = height;
  m_updateproj = true;
}

void 
OpenglContext::SetFov (double fov)
{
  m_fov = fov;
  m_updateproj = true;
}

void 
OpenglContext::SetClipDistance (double dnear, double dfar) 
{
  m_near = dnear;
  m_far = dfar;
  m_updateproj = true;
}

void 
OpenglContext::SetDepthTest (bool state){
  m_depthtest = state;
  if( !state )
    glDisable( GL_DEPTH_TEST );
  else{
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );   
    glEnable( GL_DEPTH_TEST );
  }
}

void 
OpenglContext::SetLighting (bool state)
{
  m_lightstate = state;
  if (m_lightstate)
  {
    glEnable (GL_NORMALIZE);
    glEnable (GL_LIGHTING);
    glPushMatrix ();
     glLoadIdentity ();
     gluLookAt (0, 0, 2, 0, 0, 0, 0, 1, 0);
     m_light.Enable();
     m_light.ToOpenGL();
     m_material.ToOpenGL();
    glPopMatrix ();
  }
  else
  {
    glDisable (GL_NORMALIZE);
    glDisable (GL_LIGHTING);
  }
}

void 
OpenglContext::SetLightType (OpenglContext::LightType type)
{
  m_lighttype = type;

  switch (type) 
  {
  case LIGHT_FLAT:
    glShadeModel (GL_FLAT);
    break;
  case LIGHT_SMOOTH:
    glShadeModel (GL_SMOOTH);
    break;
  default:
    assert(0);
    break;
  }
}

void 
OpenglContext::ShowLightPosition (bool flag)
{
  m_lightdraw = flag;
}

void 
OpenglContext::MoveLight (int anglex, int angley, double distance) {
  glEnable (GL_NORMALIZE);
  glPushMatrix ();
   glLoadIdentity ();
   gluLookAt (0, 0, 2, 0, 0, 0, 0, 1, 0);
   m_light.MoveLight (anglex, angley, distance);
   m_light.ToOpenGL();
  glPopMatrix ();
}

void 
OpenglContext::ChangeShininess (float change)
{
  m_material.ChangeShininess (change);
}

void 
OpenglContext::ChangeDiffuse (float changex, float changey, float changez)
{
  m_light.ChangeDiffuse (changex, changey, changez);
}

void 
OpenglContext::ChangeSpecular (float changex, float changey, float changez)
{
  m_material.ChangeSpecular (changex, changey, changez);
}

void 
OpenglContext::OppositeColorFlags ()
{
  m_colorflag = !m_colorflag;

  if (m_colorflag)
  {
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
  }
  else
    glDisable(GL_COLOR_MATERIAL);
}

void 
OpenglContext::OppositePolygonMode ()
{
  m_polygonmode = ! m_polygonmode;

  if (m_polygonmode)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void
OpenglContext::DrawHelp () {
  int cury=0;

  glDisable(GL_DEPTH_TEST);
  glColor3f( 1.0, 1.0, 1.0 );

  for (unsigned int i = 0; i < sizeof(helpInfos)/sizeof(helpStruct); i++) 
  {
    QString tmp(helpInfos[i].key);
    tmp += " ";
    tmp += helpInfos[i].helpString;
    m_parent->renderText (10, cury+=m_font.pixelSize(), tmp, m_font);
  }
}

void 
OpenglContext::DrawHud()
{
  glPushAttrib (GL_ENABLE_BIT);
  glDisable (GL_LIGHTING);

  int starty = m_font.pixelSize()*sizeof(helpInfos)/sizeof(helpStruct);
  
  /* Draw the light position using 2 quads */
  if (m_lightdraw){
    glPushMatrix ();  
     glLoadIdentity ();
     gluLookAt (0, 0, 2, 0, 0, 0, 0, 1, 0);
     glScalef (m_zoomfactor, m_zoomfactor, m_zoomfactor);
     m_light.Draw();
    glPopMatrix ();
  }

  /* Disable zbuffer for text rendering */
  glDisable (GL_DEPTH_TEST);
  
  glColor3f (1.0, 1.0, 1.0);
  if (m_showstats)
  {
    Vec3f tmp;
    QString spolys ("Triangles ");
    QString sshininess ("Shininess ");
    QString sdiffuse ("Diffuse (");
    QString sspecular ("Specular (");
    VertexBuffer *vb = m_parent->getVertexBuffer ();
    spolys += QString::number (vb->getSize ());
    m_parent->renderText (10, starty+40, spolys, m_font);
    sshininess += QString::number (m_material.GetShininess());
    m_parent->renderText (10, starty+50, sshininess, m_font);
    tmp = m_light.GetDiffuse();
    sdiffuse += QString::number(tmp.x) + ", ";
    sdiffuse += QString::number(tmp.y) + ", ";
    sdiffuse += QString::number(tmp.z) + " )";
    m_parent->renderText (10, starty+60, sdiffuse, m_font);
    tmp = m_material.GetSpecular();
    sspecular += QString::number (tmp.x) + ", ";
    sspecular += QString::number (tmp.y) + ", ";
    sspecular += QString::number (tmp.z) + " )";
    m_parent->renderText (10, starty+70, sspecular, m_font);
  }

  if (m_showfps)
  {
    int curtime;
    QString sfps ("FPS: ");
    QTime t = QTime::currentTime ();
    curtime = (t.hour()*3600 + t.minute()*60 + t.second())*1000 + t.msec();
    m_frames++;
    
    if (m_lasttime == -1) 
    {
      m_lasttime = curtime;
      m_frames = 0;
      m_fps = 0.0f;
    }
    
    if ((curtime - m_lasttime) > 500)
    {
      m_fps = 1000.0 * (float)m_frames /(curtime - m_lasttime);
      m_frames = 0;
      m_lasttime = curtime;
    }
    
    sfps += QString::number (m_fps);
    m_parent->renderText (10,  starty+30, sfps, m_font);
  }
  
  if (m_showhelp)
    DrawHelp ();
  
  /* Restore gl flags */
  glPopAttrib ();
  
}

void 
OpenglContext::ShowFps( bool flag )
{
  m_showfps = flag;
  if (!m_showfps)
    m_lasttime = -1;
}

void 
OpenglContext::ShowStats (bool flag)
{
  m_showstats = flag;
}

void 
OpenglContext::SyncContext () 
{
  if (m_updateproj)
  {
    m_updateproj = false;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (m_fov, m_viewaspect, m_near, m_far);
    glMatrixMode (GL_MODELVIEW);
    /* Build the Projection Matrix */
  }
  
  if (m_updatemview)
  {
    m_updatemview = false;
    glMatrixMode (GL_MODELVIEW);
    m_tball->getRotation (m_modelview);
    /* Send the Matrix */
    glLoadIdentity ();
    gluLookAt (0, 0, 2, 0, 0, 0, 0, 1, 0);
    glMultMatrixf ((float *)m_modelview[0]);
    /* Apply Zoom */
    glScalef (m_zoomfactor, m_zoomfactor, m_zoomfactor);
  }
}
