/**
 * @file   Solver.cc
 * @author Irek Tobor, Patrick Reuter
 * @date   Mon Apr  5 20:49:19 2004
 * 
 * @brief  Solver 
 * 
 * $Log: Solver.cc,v $
 * Revision 1.5  2004/04/28 19:20:12  pumpkins
 * code cleanup
 *
 * Revision 1.4  2004/04/05 19:14:36  pumpkins
 * File documentation
 *
 * 
 */
#include <fstream>

#include "Solver.h"
#include "math/vector3.h"
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
