#ifndef MATRIX3_H
#define MATRIX3_H

#include <string>

/* C++ implementation of Matrix3 */

template<class T> class Matrix3{
 public:
  
  Matrix3(){}
  Matrix3( T x ){ 
    data[0][0] = x; data[0][1] = x; data[0][2] = x;
    data[1][0] = x; data[1][1] = x; data[1][2] = x;
    data[2][0] = x; data[2][1] = x; data[2][2] = x;
  }

  Matrix3( Matrix3<T>& m ) {
    memcpy( data, m.data, 9*sizeof( T ) );
  }

  inline void Zero() {
    memset( data, 0, 9*sizeof( T ) );
  }

  inline void Identity() { 
    memset( data, 0, 9 * sizeof( T ) );
    data[0][0] = 1.0; data[1][1] = 1.0; data[2][2] = 1.0;
  }

  inline Matrix3<T>& operator*( Matrix3<T>& m ) {
    Matrix3<T> *o = new Matrix3<T>;
    for( int i=0; i<3; i++){
      o->data[i][0] = data[i][0] * m.data[0][0] +
	data[i][1] * m.data[1][0] + data[i][2] * m.data[2][0];
      o->data[i][1] = data[i][0] * m.data[0][1] +
	data[i][1] * m.data[1][1] + data[i][2] * m.data[2][1];
      o->data[i][2] = data[i][0] * m.data[0][2] +
	data[i][1] * m.data[1][2] + data[i][2] * m.data[2][2];
    }
    return *o;
  }

  inline Matrix3<T>& operator+( Matrix3<T>& m ) {
    Matrix3<T> *o = new Matrix3<T>;    
    for( int i=0; i<3; i++){
      o->data[i][0] = m.data[i][0] + data[i][0];
      o->data[i][1] = m.data[i][1] + data[i][1];
      o->data[i][2] = m.data[i][2] + data[i][2];			     
    }
    return *o;
  }

  inline Matrix3<T>& operator-( Matrix3<T>& m ) {
    Matrix3<T> *o = new Matrix3<T>;    
    for( int i=0; i<3; i++){
      o->data[i][0] = m.data[i][0] - data[i][0];
      o->data[i][1] = m.data[i][1] - data[i][1];
      o->data[i][2] = m.data[i][2] - data[i][2];			     
    }
    return *o;
  }

  inline Matrix3<T>& operator+=( Matrix3<T>& m ) {
    for( int i=0; i<3; i++){
      data[i][0] += m.data[i][0] ;
      data[i][1] += m.data[i][1] ;
      data[i][2] += m.data[i][2] ;			     
    }
    return *this;
  }

  inline Matrix3<T>& operator-=( Matrix3<T>& m ) {
    for( int i=0; i<3; i++){
      data[i][0] -= m.data[i][0] ;
      data[i][1] -= m.data[i][1] ;
      data[i][2] -= m.data[i][2] ;			     
    }
    return *this;
  }

  inline Matrix3<T>& operator*=( Matrix3<T>& m ) {
    T a,b,c;
    for( int i=0; i<3; i++){
      a = data[i][0]; b = data[i][1]; c = data[i][2];
      data[i][0] = a * m.data[0][0] + b * m.data[1][0] + c * m.data[2][0];
      data[i][1] = a * m.data[0][1] + b * m.data[1][1] + c * m.data[2][1];
      data[i][2] = a * m.data[0][2] + b * m.data[1][2] + c * m.data[2][2];
    }
    return *this;
  }

  inline Matrix3<T>& operator*=( T f ) {
    data[0][0] *= f;  data[0][1] *= f;  data[0][2] *= f;
    data[1][0] *= f;  data[1][1] *= f;  data[1][2] *= f;
    data[2][0] *= f;  data[2][1] *= f;  data[2][2] *= f;
    return *this;
  }

  inline Matrix3<T>& operator/=( T f ) {
    data[0][0] /= f;  data[0][1] /= f;  data[0][2] /= f;
    data[1][0] /= f;  data[1][1] /= f;  data[1][2] /= f;
    data[2][0] /= f;  data[2][1] /= f;  data[2][2] /= f;
    return *this;
  }

  inline Matrix3<T>& operator=( Matrix3<T>& m ) {
    memcpy( data, m.data, 9*sizeof( T ));
    return *this;
  }

  inline Matrix3<T>& operator=( T f) {
    data[0][0] = f;data[1][0] = f;data[2][0] = f;
    data[0][1] = f;data[1][1] = f;data[2][1] = f;
    data[0][2] = f;data[1][2] = f;data[2][2] = f;
    return *this;
  }

  void Dump(); /* Dump to console */

 private:

  T data[3][3];
};

#endif
