/*
 *  Common Matrix functions
 *
 *  Copyright(C) 2004  Dalla Rosa Damien
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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


