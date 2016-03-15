#include <Rcpp.h>

// [[Rcpp::export]]
double pi_sample(int n) {
  int tot = 0;
  for (int i = 0; i < n; ++i) {
    double x = unif_rand(), y = unif_rand();
    if (x * x + y * y <= 1) {
      ++tot;
    }
  }
  return 4 * tot / (double) n;
}
