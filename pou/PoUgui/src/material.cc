#include "material.h"
#include "opengl.h"
#include "utils.h"

using namespace std;

Material::Material()
{
  m_ambient = Vec3f( 0.3, 0.3, 0.3 ) ;
  m_diffuse = Vec3f( 0.2, 0.2, 0.2 );
  m_specular = Vec3f( 0.5, 0.5, 0.5 );
  m_shininess = 8.0f;
}

void 
Material::ChangeShininess (float change)
{
  m_shininess = clamp(m_shininess+change, 0.0f, 128.0f);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
}

void 
Material::ChangeSpecular (float changex, float changey, float changez)
{
  m_specular.x = clamp(m_specular.x+changex, 0.0f, 1.0f);
  m_specular.y = clamp(m_specular.y+changey, 0.0f, 1.0f);
  m_specular.z = clamp( m_specular.z+changez, 0.0f, 1.0f);
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &m_specular.x );
}

void
Material::ToOpenGL()
{
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, &m_ambient.x );
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, &m_diffuse.x );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, &m_specular.x );
  glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
}
