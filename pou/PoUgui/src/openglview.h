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
  
  void SetLighting( bool state, int type, float x, float y, float z );
  
  /* Surcharge des fonctions de base */
  void initializeGL();
  void clearGL();
  void resizeGL( int w, int h );
  void paintGL();
  inline OpenglContext *getOpenGLContext(){ return glcontext; }

 private:
  void mouseReleaseEvent( QMouseEvent * e);
  void mousePressEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
  void wheelEvent ( QWheelEvent * e );
  
 private:
  VertexBuffer *vb;
  OpenglContext *glcontext;

};

class OpenglView : public QMainWindow {
 public:
  OpenglView( QWorkspace *parent, VertexBuffer *vbuffer );
  ~OpenglView();

  inline void SetLighting( bool state, int type, float x, float y, float z ){
    glwidget -> SetLighting( state, type, x, y, z );
  }
  
 protected:

 private:
  OpenglWidget *glwidget;
  QFrame *glframe;
};



#endif
