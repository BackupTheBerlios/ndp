#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include <qfont.h>

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
  void SetClipDistance (double near, double far);
  void OppositeShowHelp () { m_showhelp = !m_showhelp; }

  /****************************/
  /* DON'T NEED SyncContext() */
  /****************************/
  void SetDepthTest (bool state);
  void SetLighting (bool state);
  void SetLightType (LightType type);
  void MoveLight (int anglex, int angley, double distance);
  void SetMaterial ();
  void OppositeColorFlags ();
  void OppositePolygonMode ();

 protected:
  void mapToSphere (Vec3f &dest);  
  void DrawHelp ();

 private:
  OpenglWidget *m_parent;

  /* View */
  Matrix4f m_modelview;
  Matrix4d m_projection;
  double m_fov, m_viewaspect;
  int m_width, m_height;
  double m_far, m_near;
  bool m_updateproj, m_updatemview, m_depthtest;

  /* Camera Motion */
  double m_zoomfactor;
  Vec3f m_startVector;
  Quaternionf m_orientation, m_startOrientation;
  static const float DEF_ZOOM = 0.5;
  static const float INVSQRT2;

  /* Light Motion */
  double m_lightdistance;
  int m_lightrx, m_lightry;
  Vec3f m_lightpos;
  
  /* Lighting */
  bool m_lightstate, m_lightdraw;
  int m_lighttype;
  Vec3f m_light_ambient, m_light_diffuse, m_light_specular;

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
  static helpStruct helpInfos[];

};



#endif
