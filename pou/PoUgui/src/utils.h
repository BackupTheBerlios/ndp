#ifndef UTILS_H
#define UTILS_H

#include <cmath>

class QString;
class QWidget;

template<typename T>
T deg2rad(T x) { return (x*M_PI)/T(180); }

void CheckConfigPath();
QString getConfigPath();
void CheckOpenGL();

void 
ShowErrorMessage (QWidget *parent, const QString& msg);

#endif
