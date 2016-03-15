#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP hello_world3(SEXP rn) {
  if (LENGTH(rn) != 1) {
    error("Expected a scalar for 'n'");
  }
  rn = PROTECT(coerceVector(rn, INTSXP));
  int n = INTEGER(rn)[0];
  if (n == NA_INTEGER) {
    error("Expected non-NA 'n' (possibly caused by coersion?)");
  }
  SEXP ret = PROTECT(allocVector(REALSXP, n));
  double *r = REAL(ret);
  GetRNGstate();
  for (int i = 0; i < n; ++i) {
    r[i] = rcauchy(0, 1);
  }
  PutRNGstate();
  UNPROTECT(2);
  return ret;
}
