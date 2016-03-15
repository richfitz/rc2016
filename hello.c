#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP hello_world() {
  GetRNGstate();
  double ret = rcauchy(0, 1);
  PutRNGstate();
  return ScalarReal(ret);
}
