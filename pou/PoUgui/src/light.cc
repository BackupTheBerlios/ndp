#include "light.h"
#include "opengl.h"
#include "utils.h"

Light::Light (unsigned int lightnum) :
  m_lightnum(lightnum+GL_LIGHT0)
{
  drawingColor.setValues (0, 1, 0);

  m_lightrx = 0;
  m_lightry = 0;
  m_lightdistance = 0;
  m_lightpos = Vec3f (0.0f, 0.0f, 0.0f);
  m_diffuse = Vec3f (0.2, 0.2, 0.2);
  m_specular = Vec3f (0.4, 0.4, 0.4);
  m_cst_atenuation = 1.0;
  m_lin_atenuation = 0.1;
  m_quad_atenuation = 0.1;
}

void
Light::Draw()
{
  glDisable (GL_CULL_FACE);
  glColor3f (drawingColor.x, drawingColor.y, drawingColor.z);
  glBegin (GL_QUADS);
  glVertex3f (m_lightpos.x - 0.1, m_lightpos.y - 0.1, m_lightpos.z);
  glVertex3f (m_lightpos.x + 0.1, m_lightpos.y - 0.1, m_lightpos.z);
  glVertex3f (m_lightpos.x + 0.1, m_lightpos.y + 0.1, m_lightpos.z);
  glVertex3f (m_lightpos.x - 0.1, m_lightpos.y + 0.1, m_lightpos.z);

  glVertex3f (m_lightpos.x - 0.1, m_lightpos.y, m_lightpos.z-0.1);
  glVertex3f (m_lightpos.x + 0.1, m_lightpos.y, m_lightpos.z-0.1);
  glVertex3f (m_lightpos.x + 0.1, m_lightpos.y, m_lightpos.z+0.1);
  glVertex3f (m_lightpos.x - 0.1, m_lightpos.y, m_lightpos.z+0.1);
  glEnd ();
}

void
Light::MoveLight(int anglex, int angley, double distance)
{
  double sPhi, theta, phi;
  m_lightdistance += distance;
  m_lightrx += anglex;
  m_lightry += angley;
  
  theta = deg2rad ((double)m_lightry);
  phi = deg2rad ((double)m_lightrx);

  sPhi = sin (phi);

  m_lightpos.x = m_lightdistance * cos(theta) * sPhi;
  m_lightpos.z = m_lightdistance * sin(theta) * sPhi;
  m_lightpos.y = m_lightdistance * cos(phi);
}

void
Light::ToOpenGL()
{
  float pos[4] = { m_lightpos.x , m_lightpos.y, m_lightpos.z, 1.0f };
  glLightfv (m_lightnum,GL_POSITION,(float *)pos);
  glLightfv (m_lightnum, GL_DIFFUSE, &m_diffuse.x);
  glLightfv (m_lightnum, GL_CONSTANT_ATTENUATION, &m_cst_atenuation);
  glLightfv (m_lightnum, GL_LINEAR_ATTENUATION, &m_lin_atenuation);
  glLightfv (m_lightnum, GL_QUADRATIC_ATTENUATION, &m_quad_atenuation);
  
}

void 
Light::ChangeDiffuse (float changex, float changey, float changez)
{
  m_diffuse.x = clamp (m_diffuse.x+changex, 0.0f, 1.0f);
  m_diffuse.y = clamp (m_diffuse.y+changey, 0.0f, 1.0f);
  m_diffuse.z = clamp (m_diffuse.z+changez, 0.0f, 1.0f);
  glLightfv (m_lightnum, GL_DIFFUSE, &m_diffuse.x);
}

void
Light::Enable ()
{
  glEnable (m_lightnum);
}
