/*
 *  MWindow class
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
 *  Sunday 28 March 2004:
 *      - Motion support (Dalla Rosa Damien )
 */

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qkeysequence.h>
#include <qworkspace.h>
#include <qvbox.h>
#include <qfiledialog.h>
#include <qpixmap.h>

#include <iostream>

#include "libgui.h"
#include "opengl.h"
#include "ImplicitSurface3D.h"
#include "PointSet.h"
//#include "ims.h"

/*****************************************************************************/
// vertex buffers
/*****************************************************************************/
VertexBuffer *vbPoints = NULL;
VertexBuffer *vbPolys = NULL;
int nPoints;
int nPolys;
/*****************************************************************************/
// ImpliciteSurface3D API class
/*****************************************************************************/
ImplicitSurface3D *ims;
/*****************************************************************************/
// Main Window class
/*****************************************************************************/

QWorkspace *MW_workspace;

MWindow::MWindow() 
  : QMainWindow( 0, NULL, 0 )
{
  CheckOpenGL();
  CreateMenu();
  CreateWorkspace();
}

MWindow::MWindow( QString title )
  : QMainWindow( 0, title, 0 ) 
{
  CheckOpenGL();
  CreateMenu();
  CreateWorkspace();
}

MWindow::~MWindow() {
}

void AddTool( QObject *parent, char *text, QPixmap icon, QKeySequence key,
	      QToolBar *toolbar, QPopupMenu *menu, char *slot ){
  QAction * action = new QAction( text, icon, text, key, parent, text );
  if( toolbar )
    action->addTo( toolbar );
  if( menu )
    action->addTo( menu );
  if( slot )
    parent->connect( action, SIGNAL( activated() ), parent, slot );
}

void MWindow::CreateMenu() {
  QToolBar *toolbar = new QToolBar( this, "ToolBar" );
  QPopupMenu *menu_file = new QPopupMenu( this );
  QPopupMenu *menu_windows = new QPopupMenu( this );
  QPopupMenu *menu_rendering = new QPopupMenu( this );
  QPopupMenu *menu_settings = new QPopupMenu( this );
  QPopupMenu *menu_help = new QPopupMenu( this );

  toolbar->setLabel( "ToolBar" );
  menuBar()->insertItem( MENU_FILE, menu_file );
  menuBar()->insertItem( MENU_WINDOWS, menu_windows );
  menuBar()->insertItem( MENU_RENDERING, menu_rendering );
  menuBar()->insertItem( MENU_SETTINGS, menu_settings );
  menuBar()->insertItem( MENU_HELP, menu_help );
  //Files
  AddTool( this, MENU_FILE_OPEN, QPixmap("../icons/fileopen.png"),
	   QKeySequence("Ctrl+O"), toolbar, menu_file,SLOT(menu_file_open()));
  AddTool( this, MENU_FILE_CLOSE, QPixmap("../icons/fileclose.png"),
	   QKeySequence("Ctrl+D"), toolbar, menu_file,SLOT(menu_file_close()));
  menu_file->insertSeparator();
  AddTool( this, MENU_FILE_EXIT, QPixmap("../icons/exit.png"),
	   QKeySequence("Ctrl+X"), NULL, menu_file,SLOT(menu_file_exit()));
  //Windows
  toolbar->addSeparator();
  AddTool( this, MENU_WINDOWS_NEW, QPixmap("../icons/window_new.png"),
	   QKeySequence("Ctrl+N"), toolbar, menu_windows, 
	   SLOT(menu_windows_new()));
  // Settings
  toolbar->addSeparator();
  AddTool( this, MENU_SETTINGS_ARGS, QPixmap("../icons/configure.png"),
	   QKeySequence("Ctrl+S"), toolbar, menu_settings, 
	   SLOT(menu_settings_args()));
  //Rendering
  toolbar->addSeparator();
  AddTool( this, MENU_RENDERING_RENDER, QPixmap("../icons/render.png"),
	   QKeySequence("Ctrl+R"), toolbar, menu_rendering, 
	   SLOT(menu_rendering_render()));
}

void MWindow::CreateWorkspace() {
  QVBox *vbox = new QVBox( this );
  MW_workspace = new QWorkspace( vbox );
  MW_workspace -> setScrollBarsEnabled( TRUE );
  this -> setCentralWidget( vbox );
}

void MWindow::CloseWindows() {
  QWidgetList windows = MW_workspace->windowList();
  if ( windows.count() ) {
    for ( int i = 0; i < int(windows.count()); ++i ) {
      QWidget *window = windows.at( i );
      if ( !window->close() )
	return;
    }
  }
}

void MWindow::closeEvent( QCloseEvent *event ) {
  CloseWindows();
  QMainWindow::closeEvent( event );
}

void MWindow::menu_file_open() {
  /*TODO: à virer*/
  int i;
  QString filename = QFileDialog::
    getOpenFileName( QString::null, "Fichiers .sur (*.sur)", this,"file open", 
		     "Sur -- Ouvrir Fichier" );

  if( !filename.isEmpty() ){
    /* TODO REMOVE: getPoints() is missing in ImplictSurface3D (thomas :-) )*/
    PointSet *ps;
    Vec3f *vPoints;
    Point *p;
    int step = sizeof(Point) / sizeof(Vec3f);

    ps = new PointSet();    
    ps -> load( filename ); 
    PointList::iterator psiterator = ps->getBegin();
    nPoints = ps->size(); 
    vPoints = new Vec3f [ nPoints * step];
    
    for( i=0; i<nPoints; i++, psiterator++){
      p = *psiterator;
      memcpy( (void *)&vPoints[i*step], (void *)p,sizeof( Point ) );
    }

    vbPoints = new VertexBuffer();
    vbPoints -> CreateVertexBuffer( vPoints, nPoints,step, POLY_POINTS );
    
    OpenglView *mw = new OpenglView( MW_workspace, vbPoints );
    mw -> resize( 200, 200 );
    mw -> show();
  }
}

void MWindow::menu_file_exit() {
  close();
}

void MWindow::menu_file_close() {
  CloseWindows();
  //Object *obj = GetMainObject();
  //if( obj )
  //delete obj;
  //SetMainObject( NULL );
}

void MWindow::menu_settings_args() {
}

void MWindow::menu_windows_new() {
}

void MWindow::menu_rendering_render() {
}

/*****************************************************************************/
// Create the main Window
/*****************************************************************************/

QMainWindow *MW_window;
QApplication *QT_Gui;

int CreateMainWindow( int argc, char **argv ) {
  /* First check if api is OK */
  ims = new ImplicitSurface3D();
  if( !ims )
    return -1;
  /* Create the QT Application */
  QT_Gui = new QApplication( argc, argv );
  /* Create Main Window */
  MW_window = new MWindow( " PoU GUI " );
  MW_window -> resize( 640, 480 );
  MW_window -> setCaption( "PoU GUI" );
  QT_Gui -> setMainWidget( MW_window );
  MW_window -> show();

  return QT_Gui->exec();
}
