#ifndef STRUCTUREPARSER_H
#define STRUCTUREPARSER_H

#include <qxml.h>
#include <qstring.h>
#include <vector>
#include <iostream>

class XMLEntry {
 public:
  QString tag;
  QString name;
  QString value;
};

typedef void (*XMLCallback)(const QString& tagname, const QString& value);

class XMLParser : public QXmlDefaultHandler
{
 public:
  XMLParser (QString filename);
  ~XMLParser ();
  
  int ParseFile (XMLCallback tagcallback);
  int SaveFile (const QString& filename, const QString& data);

 protected:

  bool startDocument ();
  bool startElement (const QString&, const QString&, const QString& ,
		     const QXmlAttributes&);
  bool endElement (const QString&, const QString&, const QString&);
  bool characters (const QString&);

 private:
  QString m_filename;
  QString m_tagname;
  XMLCallback m_tagcallback;
};

#endif
				       
