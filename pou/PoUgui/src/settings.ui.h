/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include <stdio.h>
#include <iostream>
#include <qfiledialog.h>
#include "xmlparser.h"
#include "utils.h"
#include "settings.h"

/* OK */
void 
SettingsForm::pushButton3_clicked()
{	
    if (!RetreiveValues())
	hide();
}

/* CANCEL */
void 
SettingsForm::pushButton3_2_clicked()
{
    ApplyValues ();
    hide();
}

/* LOAD */
void 
SettingsForm::pushButton1_clicked()
{
    QString filename = QFileDialog::getOpenFileName (getConfigPath(), "Settings Files (*.xml)", this,
						     "file open",  "Settings  -- Open File");
    if (!filename.isEmpty())
	LoadFile (filename);
}

/*SAVE*/
void 
SettingsForm::pushButton1_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName (getConfigPath(), "Settings Files (*.xml)", this,
						     "file save",  "Settings  -- Save File");    
    if (!filename.isEmpty())
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

int SettingsForm::getTmin()
{
    return m_tmin;
}


int SettingsForm::getTmax()
{
    return m_tmax;
}


int SettingsForm::getPhi()
{
    return m_phi;
}

void 
SettingsForm::Init()
{
    m_pointscount = 3000;
    m_cubesize = 0.05;
    m_maxiteration = 10;
    m_enabletet = false;
    m_tmin = 50;
    m_tmax = 100;
    m_phi = 0;
    ApplyValues();
}

int 
SettingsForm::LoadFile( QString filename )
{    
    XMLParser xmlparser(filename);
    if (xmlparser.ParseFile () == -1)
	ShowErrorMessage (this, QString("Error Loading ")+filename);
    else {
	XMLTagParser ((void *)&xmlparser);
	ApplyValues ();
    }
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
    XMLBuffer +=QString().setNum (1);
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
    if (xmlparser.SaveFile (filename, XMLBuffer)==-1)
	ShowErrorMessage (this, QString("Error Saving file to ")+filename);
    
 return 0;
}


void 
SettingsForm::XMLTagParser (void *parser)
{
    bool noerror;
    int intvalue;
    int tmp_tmin, tmp_tmax;
    float floatvalue;
    QString tagname;
    QString value;
    XMLEntry xmlentry;
    XMLParser *xmlparser = (XMLParser *)parser;
    std::vector<XMLEntry> xmlvalues = xmlparser->getValuesVector ();
    std::vector<XMLEntry>::const_iterator xmliterator;
    
    for (xmliterator=xmlvalues.begin(); xmliterator!=xmlvalues.end(); xmliterator++)
     {
	xmlentry = *xmliterator;
	tagname = xmlentry.TagName();
	value = xmlentry.Value();
	
	if (tagname=="pointscount")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (1,-1, intvalue) ) 
		ShowErrorMessage (this, "Error Bad value for PointsCount : Valid value: [1..+oo]");		    else 
	      m_pointscount = intvalue;
	}
	
	if (tagname=="tmin")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (1,-1, intvalue) ) 
		ShowErrorMessage (this, "Error Bad value for Tmin : Valid value: [1..+oo]");		    else 
	      tmp_tmin = intvalue;
	}
	
	if (tagname=="tmax")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (1,-1, intvalue)) 
		ShowErrorMessage (this, "Error Bad value for Tmax : Valid value: [1..+oo]");		    else 
	      tmp_tmax = intvalue;
	}

	if (tagname=="phi")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (0,2, intvalue) ) 
		ShowErrorMessage (this, "Error Bad value for phi : Valid value: [1..+oo]");
	    else 
		m_phi = intvalue;
	}
	
	if (tagname=="cubesize")
	{
	    floatvalue = value.toFloat (&noerror);
	    if (!noerror || CheckFloatValue (0.0,-1.0, floatvalue) ) 
		ShowErrorMessage (this, "Error Bad value for Cube Size : Valid value: ]0..+oo]");
	    else 
		m_cubesize = floatvalue;
	}
	
	if (tagname=="maxiteration")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (1,-1, intvalue) ) 
		ShowErrorMessage (this, "Error Bad value for Max Iteration : Valid value: [1..+oo]");
	    else 
		m_maxiteration = intvalue;
	}
	
	if (tagname=="usetet")
	{
	    intvalue = value.toInt (&noerror, 10);
	    if (!noerror || CheckIntValue (0,1, intvalue) ) 
		ShowErrorMessage (this, "Error Bad value for phi : Valid value: {0,1}");
	    else 
		m_enabletet = intvalue;
	}
    }
    
    if (tmp_tmax < tmp_tmin)
	ShowErrorMessage (this, "Error Tmin > Tmax");
    else
    {
	m_tmin = tmp_tmin;
	m_tmax = tmp_tmax;
    }
    
}		

bool SettingsForm::CheckIntValue( int minvalue, int maxvalue, int value )
{
    if (value < minvalue)
	return 1;
    if (minvalue>maxvalue)
	return 0;
    return value>maxvalue;
}


bool SettingsForm::CheckFloatValue( float minvalue, float maxvalue, float value )
{
    if (value < minvalue)
	return 1;
    if (minvalue>maxvalue)
	return 0;
    return value>maxvalue;
}

bool SettingsForm::RetreiveValues()
{
    QString spointscount  = entry_pou_pointscount->text();
    QString stmin = entry_pou_tmin->text();
    QString stmax = entry_pou_tmax->text();
    QString scubesize = entry_mc_cs->text();
    QString smaxit = entry_mc_maxit->text();

     bool res;
     bool retvalue = false;
     int tmin, tmax;
    
    m_phi = combo_pou_phi->currentItem();
         
    int pointscount = spointscount.toInt (&res);
    if (!res || CheckIntValue (1,-1, pointscount)) 
    {
	retvalue = true;
	ShowErrorMessage ( this, "Error Bad value for Points Count: Valid value: [1..+oo]");
    }
    else
	m_pointscount = pointscount;
    
    tmin = stmin.toInt (&res);
    if (!res  || CheckIntValue (1,-1, tmin)) 
    {
	retvalue = true;
	ShowErrorMessage (this, "Error Bad value for tmin: Valid value: [1..+oo]");
    }
    
    tmax = stmax.toInt (&res);
    if (!res || CheckIntValue (1,-1, tmax) ) 
    {
	retvalue = true;
	ShowErrorMessage (this, "Error Bad value for tmax: Valid value: [1..+oo]");
    }
    
    float  cubesize = scubesize.toFloat (&res);
    if (!res || CheckFloatValue (0.0,-1.0, cubesize))
    {
	retvalue = true;
	ShowErrorMessage (this, "Error Bad value for Cube Size: Valid value: [0..+oo]");
    }
    else
	m_cubesize = cubesize;
    
    int  maxit = smaxit.toInt (&res);
    if (!res || CheckIntValue (1,-1, maxit))
    {
	retvalue = true;
	ShowErrorMessage (this, "Error Bad value for Max Iteration: Valid value: [1..+oo]");
    }
    else
	m_maxiteration = maxit;
    
    m_enabletet = entry_mc_tet-> isChecked();
    if (!retvalue) 
    {		
	if (tmax < tmin)
	{
	    ShowErrorMessage(this, "Error Tmin > Tmax");
	    retvalue = true;
	}
	else
	{
	    m_tmin = tmin;
	    m_tmax = tmax;
	}
    }
    
    return retvalue;
}

void SettingsForm::ApplyValues()
{
    entry_pou_pointscount->setText (QString().setNum (m_pointscount, 10));
    entry_pou_tmin->setText (QString().setNum (m_tmin, 10));
    entry_pou_tmax->setText (QString().setNum (m_tmax, 10));
    combo_pou_phi->setCurrentItem (m_phi);
    
    entry_mc_cs->setText (QString().setNum (m_cubesize));
    entry_mc_maxit->setText (QString().setNum (m_maxiteration, 10));	 
    
    entry_mc_tet->setChecked(m_enabletet==1);
}
