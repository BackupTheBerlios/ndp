#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include <qfont.h>

#include "math/matrix.h"
#include "math/vector3.h"
#include "math/quaternion.h"

class OpenglWidget;

class OpenglContext {
 public:
  OpenglContext( OpenglWidget *parent );
  ~OpenglContext();

  void SyncContext();
  /* Draw the Head-Up-Display */
  void DrawHud();
  void ShowFps( bool flag );
  void ShowStats( bool flag );
  inline bool getFpsState() { return m_showfps; }
  inline bool getStatsState() { return m_showstats; }
  /********************/
  /* NEED SyncContext() */
  /********************/
  void StartRotationMode( int x, int y, bool camera );
  void StopRotationMode( bool camera );
  void InitRotationMode( bool camera );
  void RotateView( int x, int y, bool camera );
  void ZoomView( double factor, bool camera );
  
  void SetViewSize( int width, int height );
  void SetFov( double fov );
  void SetClipDistance( double near, double far );
  /****************************/
  /* DON'T NEED SyncContext() */
  /****************************/
  void SetLighting( bool state );
  void SetLightType( int type );
  void SetLightPosition( float x, float y, float z );
  void DrawLightPosition( bool flag );
  
 protected:
  void mapToSphere(Vec3f &dest);  

 private:
  OpenglWidget *m_parent;
  /* View */
  Matrix4f m_modelview;
  Matrix4d m_projection;
  double m_fov;
  double m_viewaspect;
  int m_width, m_height;
  double m_far;
  double m_near;
  bool m_updateproj;
  bool m_updatemview;
  /* Camera Motion */
  double m_zoomfactor;
  Vec3f m_startVector;
  Quaternionf m_orientation, m_startOrientation;
  static const float DEF_ZOOM = 0.5;
  static const float INVSQRT2;

  /* Light Motion */
  double m_lightzoomfactor;
  Vec3f m_lightstartVector;
  Quaternionf m_lightorientation, m_lightstartOrientation;
  
  /* Lighting */
  bool m_lightstate;
  bool m_lightdraw;
  float m_lightx, m_lighty, m_lightz;
  int m_lighttype;
  /* Stats */
  bool m_showstats;
  bool m_showfps;
  float m_fps;
  int m_frames;
  int m_lasttime;
};



#endif
