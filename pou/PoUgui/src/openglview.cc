#include "libgui.h"
#include "opengl.h"
#include "vector3.h"

bool isOpenglReady = false;

/* -+-+-+-+-+-+-+-+-+ OpenglWidget +-+-+-+-+-+-+-+-+-+-+-+-*/
OpenglWidget::OpenglWidget( QWidget *parent, const char *name, 
			    VertexBuffer *vbuffer ) 
  :QGLWidget( parent, name )
{
  vb = vbuffer;
  glcontext = new OpenGLContext();
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
  gluLookAt( -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
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
  gluPerspective( 60.0, (float)width()/(float)height(), 0.1, 1500.0 );
  glMatrixMode( GL_MODELVIEW);

  paintGL();
}

void OpenglWidget::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT );
  
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
    swapBuffers();
  }
  
}

int mouse_x, mouse_y;
int mouse_angle_x=0, mouse_angle_y=0;
bool flag_move, flag_edit;

void OpenglWidget::mousePressEvent( QMouseEvent *e ){
  flag_move = false;
  flag_edit = false;

  if( e->button() == 1 ){
    flag_move = true;
    mouse_x = e->x();
    mouse_y = e->y();
  }
  if( e->button() == 1 ){
    flag_edit = true;
  }
  //printf("Mouse Buttons Pressed: %d\n", e->button() );
}

void OpenglWidget::mouseMoveEvent( QMouseEvent *e ) {
  if( flag_move == true ){
    int rel_x = e->x() - mouse_x;
    int rel_y = e->y() - mouse_y;
    int angle_x = 2 * rel_x / 6;
    int angle_y = 2 * rel_y / 6;
    mouse_angle_x = (mouse_angle_x + angle_x) % 360;    
    mouse_angle_y = (mouse_angle_y + angle_y) % 360;    
    mouse_x = e->x();
    mouse_y = e->y();
    if( angle_x || angle_y ){
      glLoadIdentity();
      gluLookAt( -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
      glRotatef( mouse_angle_y, 1.0, 0.0, 0.0 );
      glRotatef( mouse_angle_x, 0.0, 1.0, 0.0 );
      paintGL();
    }
  }
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

