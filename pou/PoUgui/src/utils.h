#ifndef UTILS_H
#define UTILS_H

#include <cmath>

class QString;
class QWidget;

template<typename T>
T deg2rad(T x) { return (x*M_PI)/T(180); }

// Clamp x into [a,b] 
template<typename T>
T clamp(T x, T a, T b) {
  return std::min(std::max(a, x), b);
}

void CheckConfigPath();
QString getConfigPath();
void CheckOpenGL();

void 
ShowErrorMessage (QWidget *parent, const QString& msg);

#endif
