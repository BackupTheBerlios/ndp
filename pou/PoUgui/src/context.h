#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "matrix.h"
#include "math/vector3.h"
#include "math/quaternion.h"

class OpenglContext {
 public:
  OpenglContext();
  ~OpenglContext();

  void SyncContext();

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
  

 protected:
  void mapToSphere(Vec3f &dest);  

 private:
  Matrix4f *m_modelview;
  Matrix4d *m_projection;
  double m_fov;
  double m_viewaspect;
  int m_width, m_height;
  double m_far;
  double m_near;
  bool m_updateproj;
  bool m_updatemview;

  double m_zoomfactor;
  Vec3f startVector;
  Quaternionf orientation, startOrientation;
  static const float DEF_ZOOM = 0.5;
};



#endif
