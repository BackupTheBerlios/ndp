#ifndef LIGHT
#define LIGHT

#include "math/vector3.h"

class Light {
private:
  /* Light Motion */
  double m_lightdistance;
  int m_lightrx, m_lightry;
  Vec3f m_lightpos;

  Vec3f m_ambient, m_diffuse, m_specular;
  float m_cst_atenuation, m_lin_atenuation;
  float m_quad_atenuation;

  Vec3f drawingColor;
  unsigned int m_lightnum;
  
public:
  Light(unsigned int lightnum);
  ~Light() {}

  void Enable();
  void Draw();
  void MoveLight(int anglex, int angley, double distance);
  void ToOpenGL();
  void ChangeDiffuse (float changex, float changey, float changez);

  const Vec3f &GetDiffuse() {
    return m_diffuse;
  }

};

#endif
