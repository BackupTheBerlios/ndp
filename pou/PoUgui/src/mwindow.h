#ifndef MWINDOW_H
#define MWINDOW_H

#include <qmainwindow.h>
#include "box3d.h"
#include "language.h"

class MWindow : public QMainWindow 
{
  Q_OBJECT
    public:
  MWindow();
  MWindow( QString title );
  ~MWindow();
  
 private:
  void CreateMenu();
  void CreateWorkspace();
  void CloseWindows();
 
 protected:
  void closeEvent( QCloseEvent * );
  
 signals:
  
  private slots:
  void menu_file_open();
  void menu_file_close();
  void menu_file_exit();
  void menu_settings_args();
  void menu_windows_new();
  void menu_rendering_render();
 private:
  void CleanMemory();

  BoxVolume m_bbox;
};

int CreateMainWindow( int argc, char **argv );


#endif
