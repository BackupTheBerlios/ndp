/*
 *  Main function
 *
 *  Copyright(C) 2004  Dalla Rosa Damien
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <qgl.h>
#include <stdio.h>
#include <unistd.h>

#include "libgui.h"


void CheckOpenGL() {
  if( !QGLFormat::hasOpenGL() ){
    printf("Fatal:\n");
    printf("Le support OpenGL n'est pas présent sur ce système.\n");
    printf("Veuillez recompiler QT avec le support Opengl\n");
    printf("Pour plus d'informations voir www.trolltech.com\n\n" );
    exit( -1 );
  }
}

int main( int argc, char **argv) {
  return CreateMainWindow( argc, argv );
}
