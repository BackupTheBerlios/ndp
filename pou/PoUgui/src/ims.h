#ifndef IMS_H
#define IMS_H

#include <qstring.h>

class Vertex3f {
 public:
  float x,y,z;
};

class IMS {
 public:
  bool Load(QString s) {
    return 0;
  }
  Vertex3f *getPoints(int *npoints) {
    Vertex3f *res=new Vertex3f[10];
    float f;
    int i;
    *npoints=10;
    for (i=0, f=0; i <10; i++, f+=0.36)
      res[i].x=res[i].y=res[i].z=f;
    return res;
  }
  
};

#endif /* IMS_H */
