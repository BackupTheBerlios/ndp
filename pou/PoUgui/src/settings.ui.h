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
#include "xmlparser.h"
#include "utils.h"
#include "settings.h"

/* OK */
void 
SettingsForm::pushButton3_clicked()
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
    
    m_enabletet = entry_mc_tet-> isChecked();
    
    hide();
}

/* CANCEL */
void 
SettingsForm::pushButton3_2_clicked()
{
    hide();
}

/* LOAD */
void 
SettingsForm::pushButton1_clicked()
{
    QString filename = QFileDialog::getOpenFileName (getConfigPath(), "Settings Files (*.xml)", this,"file open", 
						     "Settings  -- Open File");
    LoadFile (filename);
}

/*SAVE*/
void 
SettingsForm::pushButton1_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName (getConfigPath(), "Settings Files (*.xml)", this,"file save", 
						     "Settings  -- Save File");    
    SaveFile (filename);
}

int 
SettingsForm::getPointsCount()
{	
    return m_pointscount;			
}

int 
SettingsForm::getMaxIteration()
{
    return m_maxiteration;
}

float 
SettingsForm::getCubeSize()
{
    return m_cubesize;
}

bool 
SettingsForm::isTetEnable()
{
    return m_enabletet;
}

void 
SettingsForm::Init()
{
    m_pointscount = 3000;
    m_cubesize = 0.05;
    m_maxiteration = 10;
    m_enabletet = false;
    m_tmin = 3;
    m_tmax = 3;
}

int 
SettingsForm::LoadFile( QString filename )
{    
    XMLParser xmlparser (filename);
    xmlparser.ParseFile ((XMLCallback)XMLTagCallback);
    return 0;
}

int 
SettingsForm::SaveFile (QString filename)
{
    QString XMLBuffer;
    XMLBuffer = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<config>\n\t<pou>\n";
    
    XMLBuffer += "\t\t<pointscount>";
    XMLBuffer +=QString().setNum (m_pointscount, 10);
    XMLBuffer +="</pointscount>\n";
    
    XMLBuffer += "\t\t<tmin>";
    XMLBuffer +=QString().setNum (m_tmin);
    XMLBuffer +="</tmin>\n";
    
    XMLBuffer += "\t\t<tmax>";
    XMLBuffer +=QString().setNum (m_tmax);
    XMLBuffer +="</tmax>\n";
    
    XMLBuffer += "\t\t<phi>";
    XMLBuffer +=QString().setNum (m_tmax);
    XMLBuffer +="</phi>\n";
    
    XMLBuffer +="\t</pou>\n\t<mc>\n";
    
    XMLBuffer += "\t\t<cubesize>";
    XMLBuffer +=QString().setNum (m_cubesize);
    XMLBuffer +="</cubesize>\n";

    XMLBuffer += "\t\t<maxiteration>";
    XMLBuffer +=QString().setNum (m_maxiteration, 10);
    XMLBuffer +="</maxiteration>\n";
    
    XMLBuffer += "\t\t<usetet>";
    XMLBuffer +=QString().setNum (m_enabletet, 10);
    XMLBuffer +="</usetet>\n";
    
    XMLBuffer +="\t</mc>\n";
    XMLBuffer +="</config>\n";
        
    XMLParser xmlparser (filename);
    xmlparser.SaveFile (filename, XMLBuffer);
    
 return 0;
}


void 
SettingsForm::XMLTagCallback (const QString & tagname, const QString & value)
{
    printf("TAG: %s %s\n", (const char *)tagname, (const char *)value);
}
