#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <gsl/gsl_linalg.h>
#include <iostream>
#include "vector3.h"


class Solver
{
private:
  unsigned int size;
  gsl_matrix* A;
  gsl_vector* b;
  gsl_vector* x; 
  gsl_vector* r;

public:
  Solver(unsigned int _size)
  {
    size = _size;
    A = gsl_matrix_alloc(size, size);
    gsl_matrix_set_zero (A);
    b = gsl_vector_alloc(size);
    gsl_vector_set_zero (b);
    x = gsl_vector_alloc(size); 
    r = gsl_vector_alloc(size); 
  }

  ~Solver()
  {
    gsl_vector_free(r);
    gsl_vector_free(x);
    gsl_vector_free(b);
    gsl_matrix_free(A);
  }

  void setA(int i, int j, double val)
  {
    gsl_matrix_set(A, i, j, val);
    gsl_matrix_set(A, j, i, val);
  }

  void setB(int i, double val)
  {
    gsl_vector_set(b, i, val);
  }

  float getX(int i)
  {
    return gsl_vector_get(x, i);
  }
  
  int solve(void);
};

#endif
