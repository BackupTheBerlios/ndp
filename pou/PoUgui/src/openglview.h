#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <qgl.h>
#include <qmainwindow.h>
#include <qworkspace.h>
#include <qframe.h>
#include <qevent.h>

class VertexBuffer;
class OpenglContext;

class OpenglWidget : public QGLWidget 
{
 public:
  OpenglWidget( QWidget *parent, const char *name, VertexBuffer *vbuffer );
  ~OpenglWidget();
  
  void SetLighting( bool state, int type );
  void ParseKey( int key, int key_ascii );
  /* Surcharge des fonctions de base */
  void initializeGL();
  void clearGL();
  void resizeGL( int w, int h );
  void paintGL();
  inline OpenglContext *getOpenGLContext(){ return m_glcontext; }
  inline VertexBuffer *getVertexBuffer() { return m_vbuffer; }

 private:
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
};

class OpenglView : public QMainWindow {
 public:
  OpenglView( QWorkspace *parent, VertexBuffer *vbuffer );
  ~OpenglView();

  void SetLighting( bool state, int type, float x, float y, float z );

 protected:
  void closeEvent( QCloseEvent * );
  void keyPressEvent( QKeyEvent *e );
 private:
  OpenglWidget *m_glwidget;
  QFrame *m_glframe;
  bool m_fpsstate;
};



#endif
