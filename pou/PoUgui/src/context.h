#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include <qfont.h>

#include "light.h"
#include "material.h"
#include "trackball.h"
#include "math/matrix.h"
#include "math/vector3.h"
#include "math/quaternion.h"

class OpenglWidget;

class OpenglContext {
 public:
  enum LightType { LIGHT_FLAT=1, LIGHT_SMOOTH};
  
 public:
  OpenglContext (OpenglWidget *parent);
  ~OpenglContext ();

  void SyncContext ();
  /* Draw the Head-Up-Display */
  void DrawHud ();
  void ShowFps (bool flag);
  void ShowStats (bool flag);
  void ShowLightPosition (bool flag);
  inline bool getFpsState () { return m_showfps; }
  inline bool getStatsState () { return m_showstats; }
  inline bool getLightPositionState () { return m_lightdraw; }
  
  /********************/
  /* NEED SyncContext() */
  /********************/
  void StartRotationMode (int x, int y);
  void StopRotationMode ();
  void InitRotationMode ();
  void RotateView (int x, int y);
  void ZoomView (double factor);
  
  void SetViewSize (int width, int height);
  void SetFov (double fov);
  void SetClipDistance (double dnear, double dfar);
  void OppositeShowHelp () { m_showhelp = !m_showhelp; }

  /****************************/
  /* DON'T NEED SyncContext() */
  /****************************/
  void SetDepthTest (bool state);
  void SetLighting (bool state);
  void SetLight ();
  void SetLightType (LightType type);
  void MoveLight (int anglex, int angley, double distance);
  void ChangeShininess (float change);
  void ChangeDiffuse (float changex, float changey, float changez);
  void ChangeSpecular (float changex, float changey, float changez);
  void OppositeColorFlags ();
  void OppositePolygonMode ();

 protected:
  void DrawHelp ();

 private:
  OpenglWidget *m_parent;

  /* View */
  Matrix4f m_modelview;
  double m_fov, m_viewaspect;
  int m_width, m_height;
  double m_far, m_near;
  bool m_updateproj, m_updatemview, m_depthtest;

  /* Camera Motion */
  Trackball *m_tball;
  double m_zoomfactor;
  static const float DEF_ZOOM = 0.5;

  /* Lighting */
  Light m_light;
  Material m_material;
  bool m_lightstate, m_lightdraw;
  int m_lighttype;
  /* Polygon Mode*/
  bool m_polygonmode;
  /* Material*/
  bool m_colorflag;

  /* Stats */
  QFont m_font;
  bool m_showstats, m_showfps, m_showhelp;
  float m_fps;
  int m_frames, m_lasttime;
 
  struct helpStruct {
    const QChar key;
    const QString helpString;
  };
  static helpStruct helpInfos[16];

};



#endif
