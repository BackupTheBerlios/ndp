#include "libgui.h"
#include "opengl.h"

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
    /*
      for( int i=0; i< data ->GetSize(); i++ ) {
      
      glColor3f( data->Data()[i].color[0], data->Data()[i].color[1], 
      data->Data()[i].color[2] );
      
      glVertex3f( data->Data()[i].pos[0], data->Data()[i].pos[1], 
      data->Data()[i].pos[2] );
    
      }
    */
    glEnd();
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

