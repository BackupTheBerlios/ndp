/*
 *  MainWindow class
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
 * $Log: mwindow.cc,v $
 * Revision 1.53  2004/04/29 09:52:55  leserpent
 * Getdatapointer returns a pointer.
 *
 * Revision 1.52  2004/04/29 09:51:49  ob821
 * bugfix
 *
 * Revision 1.51  2004/04/29 09:30:41  ob821
 * bugfix
 *
 * Revision 1.50  2004/04/29 08:59:17  ob821
 * exception
 *
 * Revision 1.49  2004/04/29 08:49:42  ob821
 * exception
 *
 * Revision 1.48  2004/04/28 21:14:37  leserpent
 * Started to add show only selected points.
 * Call settingsform->exec() instead of show()(must wait).
 * BUG:the number of points into the vertexbuffer is not updated.
 *
 * Revision 1.47  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.46  2004/04/27 20:10:34  ob821
 * indent
 *
 * Revision 1.45  2004/04/27 18:43:37  leserpent
 * ok
 *
 * Revision 1.44  2004/04/27 18:37:10  leserpent
 * Changed authors
 *
 * Revision 1.43  2004/04/27 17:17:33  ob821
 * enableTet checkbox works
 *
 * Revision 1.42  2004/04/27 14:32:37  ob821
 * bugfix
 *
 * Revision 1.41  2004/04/27 14:05:48  pumpkins
 * if (!qpd) replaced by assert
 *
 * Revision 1.40  2004/04/26 16:28:38  ob821
 * code cleanup
 *
 * Revision 1.39  2004/04/26 08:56:16  leserpent
 * Removed unusued method and define in vertexbuffer
 *
 * Revision 1.38  2004/04/26 07:46:11  leserpent
 * doMc throws std::runtime_error.
 * doMc stop when callback returns false.
 * Compute colors before getPoints(in addtovertices).
 *
 * Revision 1.37  2004/04/25 15:53:12  leserpent
 * Renamed mc2.cc|h to mc.cc|h
 *
 * Revision 1.36  2004/04/25 15:27:46  leserpent
 * Alloc Mc on the stack
 *
 * Revision 1.35  2004/04/25 15:19:48  pumpkins
 * callback fixes
 * exception fixes
 * multiple constructrbfpou compute allowed
 *
 * Revision 1.34  2004/04/24 13:46:12  ob821
 * mc now use octree BoundingBox
 *
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
#include "mc/mc.h"
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
  QWidgetList windows = m_workspace->windowList (QWorkspace::CreationOrder);
  if (windows.count()) 
    for (int i = 0; i < int(windows.count()); ++i) {
      QWidget *window = windows.at (i);
      window->close ();
    }
}

void 
MainWindow::closeEvent( QCloseEvent *event ) 
{
  CloseWindows();
  QMainWindow::closeEvent( event );
}

void 
MainWindow::FillVector( const PointSet &ps, std::vector<Point> &points )
{
  PointList::const_iterator psend = ps.end();
  
  for( PointList::const_iterator i = ps.begin(); i != psend; ++i)
    points.push_back(**i);
}

void 
MainWindow::MenuFileOpen() 
{
  QString filename = QFileDialog::
    getOpenFileName( QString::null, "Fichiers .sur (*.sur)", this,"file open", 
		     "Sur -- Ouvrir Fichier" );

  if (!filename.isEmpty()) {
    std::vector<Point> vecPoints;

    CloseWindows();
    CleanMemory();

    m_pointset.removeDeleteAll();       // Clean a previous load()
    try
      {
	m_pointset.load( filename );
      } 
    catch(std::exception& e) 
      {
	ShowErrorMessage (this, QString (e.what ()));
	return;
      }
    m_pointset_filtered.randomFilter (m_pointset, 
				      m_settingsform->getPointsCount ());
    FillVector (m_pointset_filtered, vecPoints);

    m_points = new VertexBuffer (vecPoints, VertexBuffer::POLY_POINTS);

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

//FIXME: vertexbuffer is not modified
void 
MainWindow::MenuSettingsArgs() 
{
  unsigned int newNumPoints, oldNumPoints = m_settingsform->getPointsCount ();
  m_settingsform->exec();
  newNumPoints = m_settingsform->getPointsCount ();
  if ( (oldNumPoints != newNumPoints) && m_points) {
    m_pointset_filtered.randomFilter(m_pointset, newNumPoints);
    m_points->LockBuffer();
    std::vector<Point> *points = m_points->getDataPointer();
    points->clear();
    FillVector( m_pointset_filtered, *points );
    m_points->unLockBuffer();
  }
  std::cerr << oldNumPoints << " " << newNumPoints << std::endl;
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
  bool callback (int v, int max) {
    assert(qpd);
    if (qpd->wasCancelled()){
      delete qpd;
      qpd = NULL;
      return false;
    }
    qpd->setProgress ( 100*v/max);
    return true;
  }
}

void 
MainWindow::MenuRenderingRender() 
{
  std::vector<Point> vecPoints;
  int mc_maxit = m_settingsform->getMaxIteration ();
  float mc_cubesize = m_settingsform->getCubeSize ();
  bool enabletet = m_settingsform->isTetEnable ();
  int tmin = m_settingsform->getTmin ();
  int tmax = m_settingsform->getTmax ();
  int phi = m_settingsform->getPhi ();
  BoxVolume boundingbox;

  ConstructRBFPOU::TypeRBF id2rbftype [3] = {ConstructRBFPOU::BIHARMONIC,
					     ConstructRBFPOU::TRIHARMONIC,
					     ConstructRBFPOU::THINPLATE};

  if (!m_points)
    return;

  qpd = new QProgressDialog ("Computing colors...",
                             "Abort Rendering", 100, this, "progress", TRUE);
  m_qtgui->processEvents ();

  /* Start ImplicitSurface reconstruction */
  ImplicitSurface3D ims (id2rbftype [phi]);
  ims.setThresholds (tmin, tmax);
  ims.setCallBack (callback, 10);

  ims.computeRGB (m_pointset_filtered);
  if (!qpd)
    return;
  qpd->setLabelText ("Computing geometry...");
  ims.computeGeometry (m_pointset_filtered);

  if (!qpd)
    return;

  qpd->setLabelText ("Polygonizing surface...");
  boundingbox = ((AreaSetOctree *)ims.getOctree())->getBBox();
  Mc MCubes(callback, 10);
  MCubes.setMaxIteration (mc_maxit);
  MCubes.setCubeSize (mc_cubesize);
  MCubes.enableTet (enabletet);
  MCubes.setInitPoint (Vec3f (-1000, -1000, -1000));
  try
    {
      MCubes.doMc (&ims, boundingbox);
    } 
  catch(std::exception& e) 
    {
      ShowErrorMessage (this, QString (e.what ()));
      delete qpd;
      qpd = NULL;
      return;
    }

  if (!qpd)
    return;

  MCubes.getPoints (vecPoints);

  m_polys = new VertexBuffer (vecPoints, VertexBuffer::POLY_TRIANGLES);
  m_polys->SetIndices (MCubes.getIndices());

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
CreateMainWindow (int argc, char **argv) 
{
  QMainWindow *mainwindow;

  /* Create the QT Application */
  MainWindow::m_qtgui = new QApplication (argc, argv);

  /* Create Main Window */
  mainwindow = new MainWindow (" PoU GUI ");
  mainwindow->resize (640, 480);
  mainwindow->setCaption ("PoU GUI");
  MainWindow::m_qtgui->setMainWidget (mainwindow);
  mainwindow->show ();

  return MainWindow::m_qtgui->exec ();
}
