#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H

#include <qgl.h>
#include <qmainwindow.h>
#include <qworkspace.h>
#include <qframe.h>
#include <qevent.h>

class VertexBuffer;
class OpenGLContext;

class OpenglWidget : public QGLWidget 
{
 public:
  OpenglWidget( QWidget *parent, const char *name, VertexBuffer *vbuffer );
  ~OpenglWidget();
  /* Surcharge des fonctions de base */
  void initializeGL();
  void clearGL();
  void resizeGL( int w, int h );
  void paintGL();
  inline OpenGLContext *getOpenGLContext(){ return glcontext; }

 private:
  void mousePressEvent( QMouseEvent *e );
  void mouseMoveEvent( QMouseEvent *e );
    
 private:
  VertexBuffer *vb;
  OpenGLContext *glcontext;
};

class OpenglView : public QMainWindow {
 public:
  OpenglView( QWorkspace *parent, VertexBuffer *vbuffer );
  ~OpenglView();
 protected:

 private:
  OpenglWidget *glwidget;
  QFrame *glframe;
};



#endif
