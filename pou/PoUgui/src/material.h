#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vector3.h"

class Material {
private:
  Vec3f m_ambient, m_diffuse, m_specular;
  float m_shininess;

public:
  Material();
  ~Material() {}
  
  void ChangeShininess (float change);
  void ChangeSpecular (float changex, float changey, float changez);
  void ChangeDiffuse (float changex, float changey, float changez);
  void ToOpenGL();

  float GetShininess() {
    return m_shininess;
  }

  const Vec3f &GetSpecular() {
    return m_specular;
  }
};

#endif
