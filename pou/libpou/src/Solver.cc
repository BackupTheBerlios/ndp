#include <fstream>

#include "Solver.h"
#include "vector3.h"
#include "box3d.h"

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>


int  
Solver::solve(void)
{
  int result = gsl_linalg_HH_solve(A,b,x);
  return result;
}



