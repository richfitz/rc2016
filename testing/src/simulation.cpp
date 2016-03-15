#include <Rcpp.h>

// Pick a cell, and set its 4 cardinal neighbours to be same type as
// it is, assuming a toroid.
int clamp(int n, int max) {
  if (n < 0) {
    return max - 1;
  } else if (n < max) {
    return n;
  } else {
    return 0;
  }
}

// [[Rcpp::export]]
std::vector<int> update_model(std::vector<int> x, int nr, int nc) {
  int i = unif_rand() * nr, j = unif_rand() * nc;
  int t = x[i + j * nr];
  x[clamp(i - 1, nr) + j * nr] = t;
  x[clamp(i + 1, nr) + j * nr] = t;
  x[i + clamp(j - 1, nc) * nr] = t;
  x[i + clamp(j + 1, nc) * nr] = t;
  return x;
}

void update_model_ref(std::vector<int>& x, int nr, int nc) {
  int i = unif_rand() * nr, j = unif_rand() * nc;
  int t = x[i + j * nr];
  x[clamp(i - 1, nr) + j * nr] = t;
  x[clamp(i + 1, nr) + j * nr] = t;
  x[i + clamp(j - 1, nc) * nr] = t;
  x[i + clamp(j + 1, nc) * nr] = t;
}

// [[Rcpp::export]]
std::vector<int> update_model2(int n, std::vector<int> x,
                               int nr, int nc) {
  for (int i = 0; i < n; ++i) {
    x = update_model(x, nr, nc);
  }
  return x;
}

// [[Rcpp::export]]
std::vector<int> update_model3(int n, std::vector<int> x,
                               int nr, int nc) {
  for (int i = 0; i < n; ++i) {
    update_model_ref(x, nr, nc);
  }
  return x;
}

class universe {
public:
  std::vector<int> x;
  int nr;
  int nc;
  universe(std::vector<int> x_, int nr_, int nc_) :
    x(x_), nr(nr_), nc(nc_) {}
  void update() {
    update_model_ref(x, nr, nc);
  }
};

// [[Rcpp::export]]
SEXP universe_create(std::vector<int> x, int nr, int nc) {
  Rcpp::XPtr<universe> ret(new universe(x, nr, nc), true);
  return Rcpp::wrap(ret);
}

// [[Rcpp::export]]
void universe_update(SEXP obj) {
  Rcpp::XPtr<universe> ptr = Rcpp::as<Rcpp::XPtr<universe> >(obj);
  ptr->update();
}

// [[Rcpp::export]]
std::vector<int> universe_get(SEXP obj) {
  Rcpp::XPtr<universe> ptr = Rcpp::as<Rcpp::XPtr<universe> >(obj);
  return ptr->x;
}
