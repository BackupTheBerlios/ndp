#include <qgl.h>
#include <stdio.h>
#include <unistd.h>

#include "libgui.h"


void CheckOpenGL() {
  if( !QGLFormat::hasOpenGL() ){
    printf("Fatal:\n");
    printf("Le support OpenGL n'est pas pr�sent sur ce syst�me.\n");
    printf("Veuillez recompiler QT avec le support Opengl\n");
    printf("Pour plus d'informations voir www.trolltech.com\n\n" );
    exit( -1 );
  }
}

int main( int argc, char **argv) {
  return CreateMainWindow( argc, argv );
}
