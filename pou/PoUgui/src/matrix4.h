#ifndef MATRIX4_H
#define MATRIX4_H

#include <string>

/* C++ implementation of Matrix4 */

template<class T> class Matrix4{
 public:
  Matrix4(){}
  Matrix4( T x ){ 
    data[0][0] = x; data[0][1] = x; data[0][2] = x; data[0][3] = x;
    data[1][0] = x; data[1][1] = x; data[1][2] = x; data[1][3] = x;
    data[2][0] = x; data[2][1] = x; data[2][2] = x; data[2][3] = x;
    data[3][0] = x; data[3][1] = x; data[3][2] = x; data[3][3] = x;
  }

  Matrix4( Matrix4<T>& m ) {
    memcpy( data, m.data, 16*sizeof( T ) );
  }

  inline void Zero() {
    memset( data, 0, 16*sizeof( T ) );
  }

  inline void Identity() { 
    memset( data, 0, 16 * sizeof( T ) );
    data[0][0] = 1.0; data[1][1] = 1.0; data[2][2] = 1.0; data[3][3] = 1.0;
  }

  Matrix4<T>& operator*( Matrix4<T>& m ) {
    Matrix4<T> *o = new Matrix4<T>;
    for( int i=0; i<4; i++){
      o->data[i][0] = data[i][0] * m.data[0][0] + data[i][1] * m.data[1][0] + 
	data[i][2] * m.data[2][0] + data[i][3] * m.data[3][0]; 
      o->data[i][1] = data[i][0] * m.data[0][1] + data[i][1] * m.data[1][1] + 
	data[i][2] * m.data[2][1] + data[i][3] * m.data[3][1];
      o->data[i][2] = data[i][0] * m.data[0][2] + data[i][1] * m.data[1][2] + 
	data[i][2] * m.data[2][2] + data[i][3] * m.data[3][2];
      o->data[i][3] = data[i][0] * m.data[0][3] + data[i][1] * m.data[1][3] + 
	data[i][2] * m.data[2][3] + data[i][3] * m.data[3][3];
    }
    return *o;
  }
  
  Matrix4<T>& operator+( Matrix4<T>& m ) {
    Matrix4<T> *o = new Matrix4<T>;    
    for( int i=0; i<4; i++){
      o->data[i][0] = m.data[i][0] + data[i][0];
      o->data[i][1] = m.data[i][1] + data[i][1];
      o->data[i][2] = m.data[i][2] + data[i][2];
      o->data[i][3] = m.data[i][3] + data[i][3];
    }
    return *o;
  }
  
  Matrix4<T>& operator-( Matrix4<T>& m ) {
    Matrix4<T> *o = new Matrix4<T>;    
    for( int i=0; i<4; i++){
      o->data[i][0] = m.data[i][0] - data[i][0];
      o->data[i][1] = m.data[i][1] - data[i][1];
      o->data[i][2] = m.data[i][2] - data[i][2];
      o->data[i][3] = m.data[i][3] - data[i][3];
    }
    return *o;
  }

  Matrix4<T>& operator+=( Matrix4<T>& m ) {
    for( int i=0; i<4; i++){
      data[i][0] += m.data[i][0];
      data[i][1] += m.data[i][1];
      data[i][2] += m.data[i][2];			     
      data[i][3] += m.data[i][3];
    }
    return *this;
  }

  Matrix4<T>& operator-=( Matrix4<T>& m ) {
    for( int i=0; i<4; i++){
      data[i][0] -= m.data[i][0] ;
      data[i][1] -= m.data[i][1] ;
      data[i][2] -= m.data[i][2] ;
      data[i][3] -= m.data[i][3] ;
    }
    return *this;
  }

  Matrix4<T>& operator*=( Matrix4<T>& m ) {
    T a,b,c,d;
    for( int i=0; i<4; i++){
      a = data[i][0]; b = data[i][1]; c = data[i][2]; d = data[i][3];
      data[i][0] = a * m.data[0][0] + b * m.data[1][0] + c * m.data[2][0]
	+ d * m.data[3][0];
      data[i][1] = a * m.data[0][1] + b * m.data[1][1] + c * m.data[2][1]
	+ d * m.data[3][1];
      data[i][2] = a * m.data[0][2] + b * m.data[1][2] + c * m.data[2][2]
	+ d * m.data[3][2];
      data[i][3] = a * m.data[0][3] + b * m.data[1][3] + c * m.data[2][3]
	+ d * m.data[3][3];
	    
    }
    return *this;
  }
  
  Matrix4<T>& operator*=( T f ) {
    for( int i=0; i<4; i++ ){
      data[i][0] *= f; data[i][1] *= f;  
      data[i][2] *= f; data[i][3] *= f;
    }
    return *this;
  }

  Matrix4<T>& operator/=( T f ) {
    for( int i=0; i<4; i++ ){
      data[i][0] /= f; data[i][1] /= f;  
      data[i][2] /= f; data[i][3] /= f;
    }
    return *this;
  }

  Matrix4<T>& operator=( Matrix4<T>& m ) {
    memcpy( data, m.data, 16*sizeof( T ));
    return *this;
  }

  Matrix4<T>& operator=( T f) {
    for( int i=0; i<4; i++ ){
      data[i][0] = f; data[i][1] = f;  
      data[i][2] = f; data[i][3] = f;
    }
    return *this;
  }

  void Dump(); /* Dump to console */
  /*TODO: Add a methode and remove public */
 public:

  T data[4][4];
};

#endif
