/*
 *  Useful functions
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <qstring.h>
#include <qgl.h>

QString g_configpath;

/* Get the config path */
QString 
getConfigPath()
{
  return g_configpath;
}

/* Check if the config path exist */
/* if the path doesn't exist, create it */
void 
CheckConfigPath()
{
  struct stat buf;
  char *home = getenv( "HOME" );
  char fullpath[128];
  
  sprintf(fullpath, "%s/%s", home, ".PoUgui");
  if( stat( fullpath, &buf ) )
    {
      printf("[I] Creating settings path: %s\n", fullpath );
      mkdir( fullpath, S_IRUSR | S_IWUSR | S_IXUSR );
    }
  
  g_configpath = QString( fullpath );
}

/* Check if QT support opengl */
void 
CheckOpenGL() 
{
  if( !QGLFormat::hasOpenGL() )
    {
      printf("[E] Fatal:\n");
      printf("Le support OpenGL n'est pas présent sur ce système.\n");
      printf("Veuillez recompiler QT avec le support Opengl\n");
      printf("Pour plus d'informations voir www.trolltech.com\n\n" );
      exit( -1 );
    }
}
