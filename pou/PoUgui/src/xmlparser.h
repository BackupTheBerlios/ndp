#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <qxml.h>
#include <qstring.h>
#include <vector>
#include <iostream>

class XMLEntry {
 public:
  XMLEntry ();
  XMLEntry (const QString& tag, const QString& value);
  QString TagName();
  QString Value ();

 private:
  QString m_tagname;
  QString m_value;
};

class XMLParser : public QXmlDefaultHandler
{
 public:
  XMLParser (QString filename);
  ~XMLParser ();
  
  int ParseFile ();
  int SaveFile (const QString& filename, const QString& data);
  
  std::vector<XMLEntry>& getValuesVector ();
  
 protected:
  
  bool startDocument ();
  bool startElement (const QString&, const QString&, const QString& ,
		     const QXmlAttributes&);
  bool endElement (const QString&, const QString&, const QString&);
  bool characters (const QString&);
  
 private:
  QString m_filename;
  QString m_tagname;
  
  std::vector<XMLEntry> m_values;
};

#endif
				       
