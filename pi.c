#include <R.h>
#include <Rinternals.h>

SEXP pi(SEXP n) {
  GetRNGstate();
  int nn = INTEGER(coerceVector(n, INTSXP))[0];
  int tot = 0;
  for (int i = 0; i < nn; ++i) {
    double x = unif_rand(), y = unif_rand();
    if (x * x + y * y <= 1) {
      ++tot;
    }
  }
  PutRNGstate();
  return ScalarReal(4 * tot / (double) nn);
}
