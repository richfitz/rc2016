#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP hello_world2(SEXP rn) {
  int n = INTEGER(rn)[0];
  SEXP ret = PROTECT(allocVector(REALSXP, n));
  double *r = REAL(ret);
  GetRNGstate();
  for (int i = 0; i < n; ++i) {
    r[i] = rcauchy(0, 1);
  }
  PutRNGstate();
  UNPROTECT(1);
  return ret;
}
