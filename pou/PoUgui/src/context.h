#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "math/matrix.h"
#include "math/vector3.h"
#include "math/quaternion.h"

class OpenglContext {
 public:
  OpenglContext();
  ~OpenglContext();

  void SyncContext();
  /* Draw the Head-Up-Display */
  void DrawHud();
  void ShowFps( bool flag );

  /********************/
  /* NEED SyncContext() */
  /********************/
  void StartRotationMode( int x, int y );
  void StopRotationMode();
  void InitRotationMode();
  void RotateView( int x, int y );
  void ZoomView( double factor );
  
  void SetViewSize( int width, int height );
  void SetFov( double fov );
  void SetClipDistance( double near, double far );
  /****************************/
  /* DON'T NEED SyncContext() */
  /****************************/
  void SetLighting( bool state );
  void SetLightingType( int type );
  void SetLightingPosition( float x, float y, float z );
  void DrawLightingPosition( bool flag );
  
 protected:
  void mapToSphere(Vec3f &dest);  

 private:
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
  /* Motion */
  double m_zoomfactor;
  Vec3f m_startVector;
  Quaternionf m_orientation, m_startOrientation;
  static const float DEF_ZOOM = 0.5;
  static const float INVSQRT2;
  /* Lighting */
  bool m_lightstate;
  bool m_lightdraw;
  float m_lightx, m_lighty, m_lightz;
  int m_lighttype;
  /* Stats */
  bool m_showfps;
  float m_fps;
  int m_frames;
  int m_lasttime;
};



#endif
