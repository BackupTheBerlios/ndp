#include <fstream>

#include "Solver.h"
#include "vector3.h"
#include "box3d.h"

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_cblas.h>

using namespace std;

/*
//-----------------------------
#include <stdlib.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#define REAL double
int
_linalg_HH_svx (gsl_matrix * A, gsl_vector * x)
{
  if (A->size1 > A->size2)
    {
      GSL_ERROR ("System is underdetermined", GSL_EINVAL);
    }
  else if (A->size2 != x->size)
    {
      GSL_ERROR ("matrix and vector sizes must be equal", GSL_EBADLEN);
    }
  else
    {
      const size_t N = A->size1;
      const size_t M = A->size2;
      size_t i, j, k;
      REAL *d = (REAL *) malloc (N * sizeof (REAL));

      if (d == 0)
	{
	  GSL_ERROR ("could not allocate memory for workspace", GSL_ENOMEM);
	}


      for (i = 0; i < N; i++)
	{
	  const REAL aii = gsl_matrix_get (A, i, i);
	  REAL alpha;
	  REAL f;
	  REAL ak;
	  REAL max_norm = 0.0;
	  REAL r = 0.0;

	  for (k = i; k < M; k++)
	    {
	      REAL aki = gsl_matrix_get (A, k, i);
	      r += aki * aki;
	    }

	  
	  cout << "i=" << i << "  "
	       << "r=" << r << endl;
	  
	  if (r == 0.0)
	    {

	      for (int k = 0; k < M; k++)
		{
		  cout <<  gsl_matrix_get(A, k, i) << " ";
		}
	      cout << endl;

              free (d);
              GSL_ERROR ("matrix is rank deficient", GSL_ESING);
	    }

	  alpha = sqrt (r) * GSL_SIGN (aii);

	  ak = 1.0 / (r + alpha * aii);
	  gsl_matrix_set (A, i, i, aii + alpha);

	  d[i] = -alpha;

	  for (k = i + 1; k < N; k++)
	    {
	      REAL norm = 0.0;
	      f = 0.0;
	      for (j = i; j < M; j++)
		{
		  REAL ajk = gsl_matrix_get (A, j, k);
		  REAL aji = gsl_matrix_get (A, j, i);
		  norm += ajk * ajk;
		  f += ajk * aji;
		}
	      max_norm = GSL_MAX (max_norm, norm);

	      f *= ak;

	      for (j = i; j < M; j++)
		{
		  REAL ajk = gsl_matrix_get (A, j, k);
		  REAL aji = gsl_matrix_get (A, j, i);
		  gsl_matrix_set (A, j, k, ajk - f * aji);
		}
	    }

	  if (fabs (alpha) < 2.0 * GSL_DBL_EPSILON * sqrt (max_norm))
	    {
	      free (d);
	      GSL_ERROR("apparent singularity detected", GSL_ESING);
	    }


	  f = 0.0;
	  for (j = i; j < M; j++)
	    {
	      f += gsl_vector_get (x, j) * gsl_matrix_get (A, j, i);
	    }
	  f *= ak;
	  for (j = i; j < M; j++)
	    {
	      REAL xj = gsl_vector_get (x, j);
              REAL aji = gsl_matrix_get (A, j, i);
	      gsl_vector_set (x, j, xj - f * aji);
	    }
	}


      for (i = N; i > 0 && i--;)
	{
	  REAL xi = gsl_vector_get (x, i);
	  REAL sum = 0.0;
	  for (k = i + 1; k < N; k++)
	    {
	      sum += gsl_matrix_get (A, i, k) * gsl_vector_get (x, k);
	    }

	  gsl_vector_set (x, i, (xi - sum) / d[i]);
	}

      free (d);
      return GSL_SUCCESS;
    }
}

int
_linalg_HH_solve (gsl_matrix * A, const gsl_vector * b, gsl_vector * x)
{
  if (A->size1 > A->size2)
    {
      //System is underdetermined.

      GSL_ERROR ("System is underdetermined", GSL_EINVAL);
    }
  else if (A->size2 != x->size)
    {
      GSL_ERROR ("matrix and vector sizes must be equal", GSL_EBADLEN);
    }
  else
    {
      int status ;

      gsl_vector_memcpy (x, b);

      status = _linalg_HH_svx (A, x);
      
      return status ;
    }
}


//------------------------------
*/


int  
Solver::solve(void)
{
  // interface with gsl -- solve linear system
  //int s;
  /*
  gsl_permutation* p = gsl_permutation_alloc(size);

  //gsl_matrix* Acopy = gsl_matrix_alloc(size, size);
  //gsl_matrix_memcpy (Acopy, A);

  gsl_linalg_LU_decomp(A, p, &s);

  int result = gsl_linalg_LU_solve(A, p, b, x);

  //gsl_linalg_LU_refine(Acopy, A, p, b, x, r);
  //gsl_linalg_LU_refine(Acopy, A, p, b, x, r);
  //gsl_linalg_LU_refine(Acopy, A, p, b, x, r);
  //gsl_linalg_LU_refine(Acopy, A, p, b, x, r);

  //gsl_matrix_free(Acopy);
  gsl_permutation_free(p);

  //gsl_vector* y = gsl_vector_alloc(size);
  //gsl_vector_set_zero(y);
  //gsl_blas_sgemv(CblasNoTrans, 1, &(A.matrix), x, 0, y);
  
  //gsl_vector_free(y);
  
  */
  
  //gsl_linalg_cholesky_decomp(A);
  //int result = gsl_linalg_cholesky_solve(A,b,x);
  
  //return result;
  
  /*
  ofstream file("matrix");
  for(int i=0; i<size; i++)
    {
      for(int j=0; j<size; j++)
	{
	  double val=gsl_matrix_get(A,i,j);
	  file << val << "  ";
	}
      file << endl;
    }
  file.close();
  */

  int result = gsl_linalg_HH_solve(A,b,x);

  return result;
}



