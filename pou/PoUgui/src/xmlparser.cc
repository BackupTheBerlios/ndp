#include "settings.h"
#include "xmlparser.h"

// XML Entry
XMLEntry::XMLEntry()
{
}

XMLEntry::XMLEntry (const QString& tag, const QString& value)
{
  m_tagname = tag;
  m_value = value;
}

QString
XMLEntry::TagName()
{
  return m_tagname;
}

QString
XMLEntry::Value ()
{
  return m_value;
}

// XML Parser

XMLParser::XMLParser (QString filename)
{
  m_filename = filename;
}

XMLParser::~XMLParser()
{
}

int
XMLParser::ParseFile ()
{
  XMLParser *handler = this;
  QFile file (m_filename);
  if (file.open (IO_ReadOnly)) {
    QXmlInputSource source (&file);
    QXmlSimpleReader reader;
    reader.setContentHandler (handler);
    reader.parse (source);
    file.close ();
    return 0;
  }
  file.close ();
  return -1;
}


bool 
XMLParser::startDocument() 
{
  m_values.clear ();
  return TRUE;
}

bool 
XMLParser::startElement( const QString& a, const QString& b, const QString& c,
	      const QXmlAttributes& attr )
{
  m_tagname = b;
  return TRUE;
}

bool
XMLParser::endElement( const QString& a, const QString& b, const QString& c )
{
  return TRUE;
}

bool
XMLParser::characters (const QString& a)
{
  int n=0;
  //Skip empty lines ( space, \n  and \t are entries for XML)
  while (((a[n]==' ') || (a[n]=='\n') || (a[n]=='\t')) && (a[n]!='\0'))
    n++;
  if (a[n]=='\0')
    return TRUE;

  m_values.push_back (XMLEntry (m_tagname, a));

  return TRUE;
}

int
XMLParser::SaveFile(const QString& filename, const QString& data)
{
  QFile file (filename);
  if (file.open (IO_WriteOnly)) {
    QTextStream stream (&file);
    stream << data; 
    file.close ();
    return 0;
    }
  file.close ();
  return -1;
}

std::vector<XMLEntry>&
XMLParser::getValuesVector ()
{
return m_values;
}
