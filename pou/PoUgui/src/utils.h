#ifndef UTILS_H
#define UTILS_H

class QString;
class QWidget;

#define DEG2RAD( x ) (x) * M_PI / 180.0

void CheckConfigPath();
QString getConfigPath();
void CheckOpenGL();

void 
ShowErrorMessage (QWidget *parent, const QString& msg);

#endif
