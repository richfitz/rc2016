#include <R.h>
#include <Rinternals.h>

typedef struct universe {
  int nr;
  int nc;
  int* x;
} universe;

static void universe_finalize(SEXP ptr);

int clamp(int n, int max) {
  if (n < 0) {
    return max - 1;
  } else if (n < max) {
    return n;
  } else {
    return 0;
  }
}

void update_model(universe* obj) {
  int* x = obj->x;
  int nr = obj->nr, nc = obj->nc;
  int i = unif_rand() * nr, j = unif_rand() * nc;
  int t = x[i + j * nr];
  x[clamp(i - 1, nr) + j * nr] = t;
  x[clamp(i + 1, nr) + j * nr] = t;
  x[i + clamp(j - 1, nc) * nr] = t;
  x[i + clamp(j + 1, nc) * nr] = t;
}

SEXP universe_create(SEXP x, SEXP nr_, SEXP nc_) {
  int nr = INTEGER(nr_)[0], nc = INTEGER(nc_)[0];
  universe* obj = Calloc(1, universe);
  obj->nr = nr;
  obj->nc = nc;
  obj->x = Calloc(nr * nc, int);
  memcpy(obj->x, INTEGER(x), nr * nc * sizeof(int));
  SEXP ret = PROTECT(R_MakeExternalPtr(obj, R_NilValue, R_NilValue));
  R_RegisterCFinalizer(ret, universe_finalize);
  UNPROTECT(1);
  return ret;
}

void universe_finalize(SEXP ptr) {
  universe *obj = (universe*) R_ExternalPtrAddr(ptr);
  Free(obj->x);
  Free(obj);
  R_ClearExternalPtr(ptr);
}

SEXP universe_update(SEXP ptr) {
  GetRNGstate();
  update_model((universe*) R_ExternalPtrAddr(ptr));
  PutRNGstate();
  return R_NilValue;
}

SEXP universe_get(SEXP ptr) {
  universe *obj = (universe*) R_ExternalPtrAddr(ptr);
  int len = obj->nr * obj->nc;
  SEXP ret = PROTECT(allocVector(INTSXP, len));
  memcpy(INTEGER(ret), obj->x, len * sizeof(int));
  UNPROTECT(1);
  return ret;
}
