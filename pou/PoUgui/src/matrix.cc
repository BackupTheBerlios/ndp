/* This module implements non inline functions of both matrix3 and matrix4 */

#include <stdio.h>
#include "matrix.h"


void Matrix3<double>::Dump() {
  printf("-----------------------\n");
  for( int i=0;i<3; i++ )
    printf("[ %2f . %2f . %2f ]\n", data[i][0], data[i][1], data[i][2] );
  printf("-----------------------\n");
  }

void Matrix4<double>::Dump() {
  printf("-----------------------\n");
  for( int i=0;i<4; i++ )
    printf("[ %2f . %2f . %2f . %2f ]\n", data[i][0], data[i][1],
	   data[i][2], data[i][3] );
  printf("-----------------------\n");
}


