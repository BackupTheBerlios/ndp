#include "xmlparser.h"

XMLParser::XMLParser (QString filename)
{
  m_filename = filename;
}

XMLParser::~XMLParser()
{
}

int
XMLParser::ParseFile (XMLCallback tagcallback)
{
  m_tagcallback = tagcallback;

  XMLParser *handler = this;
  QFile file (m_filename);
  QXmlInputSource source (&file);
  QXmlSimpleReader reader;
  reader.setContentHandler (handler);
  reader.parse (source);
  return 0;
}


bool 
XMLParser::startDocument() 
{
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
  //Skip empty lines ( space and \n are entries for XML)
  while ((a[n]==' ') || (a[n]=='\n') && a[n]!='\0')
    n++;
  if (a[n]=='\0')
    return TRUE;

  m_tagcallback (m_tagname, a);

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
    }
  else
    printf("Error\n");
  return 0;
}
