#ifndef MWINDOW_H
#define MWINDOW_H

#include <qapplication.h>
#include <qmainwindow.h>
#include <qworkspace.h>

#include "PointSet.h"
#include "openglview.h"
#include "math/vector3.h"
#include "box3d.h"
#include "settings.h"
#include "language.h"

class VertexBuffer;

class MainWindow : public QMainWindow 
{
  Q_OBJECT
    public:
  MainWindow( QString title );
  ~MainWindow();
  
 private:
  void CreateMenu();
  void CreateWorkspace();
  void CloseWindows();
 
 protected:
  void closeEvent( QCloseEvent * );
 signals:
  
  private slots:
  void MenuFileOpen();
  void MenuFileClose();
  void MenuFileExit();
  void MenuSettingsArgs();
  void MenuWindowsNew();
  void MenuRenderingRender();

 private:
  void CleanMemory();

  /* Application */
  QWorkspace *m_workspace;

  /* windows / dialogs */
  OpenglView *m_polywin;
  SettingsForm *m_settingsform;
 
  /* Geometry */
  PointSet m_pointset;
  BoxVolume m_boundingbox;
  Vec3f m_initpoint;
  VertexBuffer *m_points;
  VertexBuffer *m_polys;

 public:
  static QApplication *m_qtgui;
};

int CreateMainWindow( int argc, char **argv );


#endif
