#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <qgl.h>
#include <qmainwindow.h>
#include <qworkspace.h>
#include <qframe.h>
#include <qevent.h>

#include "context.h"

class VertexBuffer;

class OpenglWidget : public QGLWidget 
{
 public:
  OpenglWidget( QWidget *parent, const char *name, VertexBuffer *vbuffer );
  ~OpenglWidget();

  inline VertexBuffer *getVertexBuffer() { return m_vbuffer; }

  void ParseKey( int key, int key_ascii );

 private:
  void SetLighting( bool state,  OpenglContext::LightType type );

  /* Surcharge des fonctions de base */
  void initializeGL();
  void clearGL();
  void resizeGL( int w, int h );
  void paintGL();
  
  void SetIdleDraw( bool state );
  void mouseReleaseEvent( QMouseEvent * e);
  void mousePressEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void wheelEvent ( QWheelEvent *e );
  void timerEvent( QTimerEvent *e ); 
 private:
  VertexBuffer *m_vbuffer;
  OpenglContext *m_glcontext;
  bool m_idledraw;
  int m_timerid;

  static bool s_openglready;
};

class OpenglView : public QMainWindow {
 public:
  OpenglView( QWorkspace *parent, VertexBuffer *vbuffer );
  ~OpenglView();

 protected:
  void closeEvent( QCloseEvent * );
  void keyPressEvent( QKeyEvent *e );
 private:
  OpenglWidget *m_glwidget;
  QFrame *m_glframe;
  bool m_fpsstate;
};



#endif
