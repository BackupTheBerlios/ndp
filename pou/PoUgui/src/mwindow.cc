/*
 *  MainWindow class
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
 * $Log: mwindow.cc,v $
 * Revision 1.33  2004/04/24 13:06:36  ob821
 * xmlparser.cc completed
 * settings.* bugfix, code cleanup
 * mwindow.cc code cleanup
 * openglview.* variables moved from global to static
 * init.cc useles
 *
 * Revision 1.32  2004/04/23 17:57:35  ob821
 * new xmlparser
 * bugfix
 * code cleanup
 *
 * Revision 1.31  2004/04/07 18:23:17  ob821
 * segfault fix
 * lighting bugfix
 *
 * Revision 1.30  2004/04/06 16:47:58  leserpent
 * Now there is only one progress dialog for colors.
 *
 * Revision 1.29  2004/04/06 16:18:29  leserpent
 * Clear PointSet before loading a new file.
 *
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
#include <qprogressdialog.h>
#include <qtimer.h>

#include <iostream>

#include "utils.h"
#include "opengl.h"
#include "mwindow.h"
#include "ImplicitSurface3D.h"
#include "mc/mc2.h"
#include "icons.h"
#include "settings.h"
#include "language.h"
#include "openglview.h"

QApplication *MainWindow::m_qtgui;

/*****************************************************************************/
// Main Window class
/*****************************************************************************/

MainWindow::MainWindow( QString title )
  : QMainWindow( 0, title, 0 ) 
{
  CheckOpenGL();
  CreateMenu();
  CreateWorkspace();

  m_polys = NULL;
  m_points = NULL;
}

MainWindow::~MainWindow() 
{
}

void 
AddTool( QObject *parent, char *text, QPixmap icon, QKeySequence key,
	 QToolBar *toolbar, QPopupMenu *menu, char *slot )
{
  QAction * action = new QAction( text, icon, text, key, parent, text );
  if( toolbar )
    action->addTo( toolbar );
  if( menu )
    action->addTo( menu );
  if( slot )
    parent->connect( action, SIGNAL( activated() ), parent, slot );
}

void 
MainWindow::CreateMenu() 
{
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
  AddTool( this, MENU_FILE_OPEN, QPixmap ((const char **)icon_fileopen),
	   QKeySequence("Ctrl+O"), toolbar, menu_file,SLOT(MenuFileOpen()));
  AddTool( this, MENU_FILE_CLOSE, QPixmap ((const char **)icon_fileclose),
	   QKeySequence("Ctrl+D"), toolbar, menu_file,SLOT(MenuFileClose()));
  menu_file->insertSeparator();
  AddTool( this, MENU_FILE_EXIT, QPixmap ((const char **)icon_exit),
	   QKeySequence("Ctrl+X"), NULL, menu_file,SLOT(MenuFileExit()));
  //Windows
  toolbar->addSeparator();
  AddTool( this, MENU_WINDOWS_NEW, QPixmap((const char **)icon_newwindow),
	   QKeySequence("Ctrl+N"), toolbar, menu_windows, 
	   SLOT(MenuWindowsNew()));
  // Settings
  toolbar->addSeparator();
  AddTool( this, MENU_SETTINGS_ARGS, QPixmap((const char **)icon_configure),
	   QKeySequence("Ctrl+S"), toolbar, menu_settings, 
	   SLOT(MenuSettingsArgs()));
  //Rendering
  toolbar->addSeparator();
  AddTool( this, MENU_RENDERING_RENDER, QPixmap((const char **)icon_render),
	   QKeySequence("Ctrl+R"), toolbar, menu_rendering, 
	   SLOT(MenuRenderingRender()));
  
  m_settingsform = new SettingsForm;
  m_settingsform->Init();
}

void 
MainWindow::CreateWorkspace() 
{
  QVBox *vbox = new QVBox( this );
  m_workspace = new QWorkspace( vbox );
  m_workspace->setScrollBarsEnabled( TRUE );
  this->setCentralWidget( vbox );
}

void 
MainWindow::CloseWindows() 
{
  QWidgetList windows = m_workspace->windowList();
  if ( windows.count() ) 
    for ( int i = 0; i < int(windows.count()); ++i ) {
      QWidget *window = windows.at( i );
      window->close();
    }
}

void 
MainWindow::closeEvent( QCloseEvent *event ) 
{
  CloseWindows();
  QMainWindow::closeEvent( event );
}

void 
MainWindow::MenuFileOpen() 
{
  QString filename = QFileDialog::
    getOpenFileName( QString::null, "Fichiers .sur (*.sur)", this,"file open", 
		     "Sur -- Ouvrir Fichier" );

  if (!filename.isEmpty()){
    std::vector<Point> vecPoints;

    CloseWindows();
    CleanMemory();

    m_pointset.removeDeleteAll();       // Clean a previous load()
    m_pointset.load( filename );
    if(0)
      {
	ShowErrorMessage (this, QString ("Error Loading ")+filename);
	return;
      }

    m_boundingbox = m_pointset.getBoundingBox();
    
    PointList::iterator psend = m_pointset.getEnd();
    
    for( PointList::iterator i=m_pointset.getBegin(); i != psend; ++i)
      vecPoints.push_back(**i);

    m_points = new VertexBuffer(vecPoints, POLY_POINTS);

    OpenglView *mw = new OpenglView( m_workspace, m_points );
    mw->resize( 200, 200 );
    mw->show();
  }
}

void 
MainWindow::MenuFileExit() 
{
  close();
}

void 
MainWindow::MenuFileClose() 
{
  CloseWindows();
  CleanMemory();
}

void 
MainWindow::MenuSettingsArgs() 
{
  m_settingsform->show();
}

void 
MainWindow::MenuWindowsNew() 
{
  if( m_points )
    {
      OpenglView *mw = new OpenglView( m_workspace, m_points );
      mw->resize( 200, 200 );
      mw->show();
    }
}

namespace 
{
  QProgressDialog *qpd;
  void callback (int v, int max) {
    qpd->setProgress ( 100*v/max);
  }
}

void 
MainWindow::MenuRenderingRender() 
{
  ImplicitSurface3D *ims;
  Mc *MCubes;
  std::vector<Point> vecPoints;
  int filter_npoints = m_settingsform->getPointsCount();
  int mc_maxit = m_settingsform->getMaxIteration();
  float mc_cubesize = m_settingsform->getCubeSize();
  bool enabletet = m_settingsform->isTetEnable();
  int tmin = m_settingsform->getTmin();
  int tmax = m_settingsform->getTmax();
  int phi = m_settingsform->getPhi();

  ConstructRBFPOU::TypeRBF id2rbftype [3] = {ConstructRBFPOU::BIHARMONIC,
					     ConstructRBFPOU::TRIHARMONIC,
					     ConstructRBFPOU::THINPLATE};
  if (!m_points)
    return;
  
  qpd = new QProgressDialog ("Computing colors...",
                             "Abort Rendering", 100, this, "progress", TRUE);
  m_qtgui->processEvents();

  /* Start ImplicitSurface reconstruction */
  /* First check if api is OK */
  ims = new ImplicitSurface3D (id2rbftype [phi]);
  if( !ims )
    return ;
  ims->setThresholds (tmin, tmax);
  ims->setCallBack (callback, 10);

  ims->computeRGB (m_pointset, filter_npoints);
  qpd->setLabelText ("Computing geometry...");
  ims->computeGeometry (m_pointset, filter_npoints);

  qpd->setLabelText ("Polygonizing surface...");
  MCubes = new Mc (callback, 10);
  MCubes->setMaxIteration (mc_maxit);
  MCubes->setCubeSize (mc_cubesize);
  MCubes->enableTet (enabletet);
  MCubes->domc (ims, m_boundingbox);
  MCubes->getPoints (vecPoints);

  m_polys = new VertexBuffer (vecPoints, POLY_TRIANGLES);
  m_polys->SetIndices (MCubes->getIndices());

  delete MCubes;
 
  m_polywin = new OpenglView (NULL, m_polys);
  m_polywin->resize (1024, 768);
  m_polywin->show();
  delete qpd;
}

void 
MainWindow::CleanMemory() 
{
  delete m_points;
  delete m_polys;
  m_points = NULL;
  m_polys = NULL;
}

/*****************************************************************************/
// Create the main Window
/*****************************************************************************/

int 
CreateMainWindow( int argc, char **argv ) 
{
  QMainWindow *mainwindow;

  /* Create the QT Application */
  MainWindow::m_qtgui = new QApplication( argc, argv );

  /* Create Main Window */
  mainwindow = new MainWindow(" PoU GUI ");
  mainwindow->resize (640, 480);
  mainwindow->setCaption ("PoU GUI");
  MainWindow::m_qtgui->setMainWidget (mainwindow);
  mainwindow->show();

  return MainWindow::m_qtgui->exec();
}
