#ifndef UTILS_H
#define UTILS_H

#include <qstring.h>

#define DEG2RAD( x ) (x) * M_PI / 180.0

void CheckConfigPath();
QString getConfigPath();
void CheckOpenGL();

#endif
