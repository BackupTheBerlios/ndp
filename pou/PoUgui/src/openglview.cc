#include <iostream>

#include "libgui.h"
#include "opengl.h"
#include "math/vector3.h"

const float OpenglWidget::DEF_ZOOM;

bool isOpenglReady = false;

/* -+-+-+-+-+-+-+-+-+ OpenglWidget +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglWidget::OpenglWidget( QWidget *parent, const char *name, 
			    VertexBuffer *vbuffer ) 
  :QGLWidget( parent, name ), zoom(DEF_ZOOM)
{
  vb = vbuffer;
  glcontext = new OpenglContext();
}

OpenglWidget::~OpenglWidget() {
  if( glcontext )
    delete glcontext;
}

void OpenglWidget::initializeGL() 
{
  //Initialise OpenGL si c'est la première utilisation
  if( !isOpenglReady )
    OpenGL_Init();
  isOpenglReady = true;
   
  clearGL();
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( 0.0, 0.0, 3, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

void OpenglWidget::clearGL() 
{
  qglClearColor( black );
}

void OpenglWidget::resizeGL( int w, int h ) 
{
  glViewport( 0, 0, (GLint)w, (GLint)h );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60.0, (float)width()/(float)height(), 0.001, 1500.0 );
  glMatrixMode( GL_MODELVIEW);

  paintGL();
}

void OpenglWidget::paintGL() {
  float m[4][4];

  glClear( GL_COLOR_BUFFER_BIT );
  
  glPushMatrix();
  orientation.unitToMatrix44(m);
  glMultMatrixf((float *)m);
  glScalef(zoom, zoom, zoom);

  if( vb ) {
    qglColor( white );

    glBegin( GL_POINTS );
    vb->LockBuffer();
    Vec3f *ptr = (Vec3f *)vb->getDataPointer();
    
    for( int i=0; i< vb -> getSize()*vb -> getStep(); i+= vb -> getStep() ) {
      glColor3f( ptr[i+2].x, ptr[i+2].y, ptr[i+2].z);
      glVertex3f( ptr[i].x, ptr[i].y, ptr[i].z );
    }
    vb->unLockBuffer();
    glEnd();
  }
  glPopMatrix();
  swapBuffers();
}

//Trouve le point (mousex',mousey',Z)
//qui se trouve sur la sphere de rayon 1
//Reference: Terence J. Grant nehe.gamedev.net
void OpenglWidget::mapToSphere(Vec3f &v) {
  Vec3f tmp;
      
  tmp.x = (2*v.x)/float(size().width()-1)-1;
  tmp.y = -((2*v.y)/float(size().height()-1)-1);
  tmp.z = 0;
  float len2 = tmp.length2();
  if(len2>1) {
    // Hors de la sphere, on prend le point le plus proche
    float len = 1.0f/std::sqrt(len2);
    tmp*=len;
  }
  else
    tmp.z = std::sqrt(1.0-len2); // Dans la sphere, on augmente Z
  v = tmp;
}

void OpenglWidget::mousePressEvent( QMouseEvent *e ) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    startVector.setValues(e->x(), e->y(), 0);
    mapToSphere(startVector);
    break;
  case QMouseEvent::RightButton: // Reset transformation
    orientation.toIdentity();
    startOrientation.toIdentity();
    zoom = DEF_ZOOM;
    updateGL();
    break;
  default:
    break;
  }    
}

void OpenglWidget::mouseReleaseEvent( QMouseEvent * e) {
  switch(e->button()) {
  case QMouseEvent::LeftButton:
    startOrientation = orientation;
    break;
  default:
    break;
  }    
}

void OpenglWidget::mouseMoveEvent( QMouseEvent *e ) {
  if(e->state()&QMouseEvent::LeftButton) {
    Quaternionf q;
    Vec3f endVector(e->x(), e->y(), 0);
    mapToSphere(endVector);
    q.toRotationArc(startVector, endVector);
    orientation = q*startOrientation;
    orientation.normalize();
    updateGL();
  }
}

void OpenglWidget::wheelEvent ( QWheelEvent * e ) {
  e->accept();
  if(e->delta()<0)
    zoom *= 0.9;
  else
    zoom *= 1.1;
  updateGL();
}

/* -+-+-+-+-+-+-+-+-+ OpenglView +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglView::OpenglView( QWorkspace *parent, VertexBuffer *vbuffer ) 
  : QMainWindow( parent, "OpenGL View", 0 )
{
  glwidget = new OpenglWidget( this, "Rendering Widget", vbuffer );
  setCaption( "OPENGL View" );
  // Permet de propager les évènements
  setCentralWidget( glwidget );
}

OpenglView::~OpenglView() 
{
  if( glwidget )
    delete glwidget;
}

