/*
 *  Useful functions
 *
 *  Copyright(C) 2004  Damien Dalla Rosa, Thomas Arcila,
 *                      Loic Martin, Benjamin Grange
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
#include <iostream>
#include <qgl.h>
#include <qmessagebox.h>

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
      std::cout<<"[E] Fatal:\n"
	       <<"Opengl not supported by QT.\n"
	       <<"Please compile QT again with opengl support\n"
	       <<"More informations on www.trolltech.com\n\n";
      exit (-1);
    }
}

int 
OpenglInit() 
{
  std::cout<<"OpenGL Informations :\n";
  std::cout<<"   OpenGL Vendor: "<< glGetString (GL_VENDOR) <<"\n";
  std::cout<<"   OpenGL Version: "<< glGetString (GL_VERSION) <<"\n";
  std::cout<<"   OpenGL Renderer: "<< glGetString (GL_RENDERER) <<"\n";

  return 0;
}

void 
ShowErrorMessage (QWidget *parent, const QString& msg )
{
  QMessageBox::warning (parent, "Error", msg, QMessageBox::Ok, 
			QMessageBox::NoButton);
}
