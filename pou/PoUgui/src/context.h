#ifndef OPENGL_CONTEXT_H
#define OPENGL_CONTEXT_H

#include "matrix.h"

class OpenglContext {
 public:
  OpenglContext();
  ~OpenglContext();

  void SyncContext();

  /********************/
  /* NEED SyncContext() */
  /********************/

  void RotateView( int relx, int rely );
  void RotateView( double rx, double ry, double rz );
  void ZoomView( int factor );
  
  void ResizeView( int width, int height );
  void SetFov( double fov );

  /* DON'T NEED SyncContext() */

 protected:
  
 private:
  Matrix4d *m_modelview;
  Matrix4d *m_projection;
  double m_zoomfactor;
  double m_fov;
  double m_viewaspect;
  bool m_updateproj;
  bool m_updatemview;
};



#endif
