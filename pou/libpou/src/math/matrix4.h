#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
#include <string>
#include <cassert>

/* C++ implementation of Matrix4 */

template<class T> class Matrix4{
 public:
  Matrix4(){}
  Matrix4( T x ){ 
    coeff[0][0] = x; coeff[0][1] = x; coeff[0][2] = x; coeff[0][3] = x;
    coeff[1][0] = x; coeff[1][1] = x; coeff[1][2] = x; coeff[1][3] = x;
    coeff[2][0] = x; coeff[2][1] = x; coeff[2][2] = x; coeff[2][3] = x;
    coeff[3][0] = x; coeff[3][1] = x; coeff[3][2] = x; coeff[3][3] = x;
  }

  inline void Zero() {
    memset( coeff, 0, 16*sizeof( T ) );
  }

  inline void Identity() {
    Zero();
    coeff[0][0] = 1.0; coeff[1][1] = 1.0; coeff[2][2] = 1.0; coeff[3][3] = 1.0;
  }

  T * operator[] (unsigned int i) {
    assert(i<4);
    return coeff[i];
  }

  friend std::ostream& operator<<(std::ostream& os, const Matrix4<T>& m) {
    os << "-----------------------" << std::endl;
    for( int i=0; i<4; i++ ) {
      os << "[ ";
      for( int j=0; j<4; j++ )
        os << m.coeff[i][j] << " ";
      os << "]" << std::endl;
    }
    os << "-----------------------" << std::endl;
    return os;
  }

private:
  T coeff[4][4];
};

#endif
