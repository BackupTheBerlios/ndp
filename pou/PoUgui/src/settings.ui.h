/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <stdio.h>
#include <qfiledialog.h>
#include "settings.h"

/* OK */
void SettingsForm::pushButton3_clicked()
{	
    QString spointscount  = lineEdit1->text();
    QString scubesize = entry_mc_cs->text();
    QString smaxit = entry_mc_maxit->text();
    bool res;
    
    int pointscount = spointscount.toInt( &res );
     if( res )
	m_pointscount = pointscount;
    
    float  cubesize = scubesize.toFloat( &res );
    if( res )
	m_cubesize = cubesize;
    
    int  maxit = smaxit.toInt( &res );
    if( res )
	m_maxiteration = maxit;
    
    hide();
}

/* CANCEL */
void SettingsForm::pushButton3_2_clicked()
{
    hide();
}
/* LOAD */
void SettingsForm::pushButton1_clicked()
{
    QString filename = QFileDialog::getOpenFileName( QString::null, "Settings Files (*.args)", this,"file open", 
					"Settings  -- Open File" );
    // TODO: Load file
}

/*SAVE*/
void SettingsForm::pushButton1_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName( QString::null, "Settings Files (*.args)", this,"file save", 
					"Settings  -- Save File" );    
    //TODO:Save file
}

int SettingsForm::getPointsCount()
{	
    return m_pointscount;			
}

int SettingsForm::getMaxIteration()
{
    return m_maxiteration;
}

float SettingsForm::getCubeSize()
{
    return m_cubesize;
}

void SettingsForm::Init()
{
    m_pointscount = 3000;
    m_cubesize = 0.05;
     m_maxiteration = 10;
}
