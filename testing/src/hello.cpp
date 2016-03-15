#include <Rcpp.h>

// This is the simple version, drawing one random number

// [[Rcpp::export]]
double hello_world() {
  return R::rcauchy(0, 1);
}

// A C++ STL version, letting Rcpp handle the type coersions

// [[Rcpp::export]]
std::vector<double> hello_world2(int n) {
  std::vector<double> ret;
  ret.reserve(n);
  for (int i = 0; i < n; ++i) {
    ret.push_back(R::rcauchy(0, 1));
  }
  return ret;
}

// An Rcpp version, using the native Rcpp types

// [[Rcpp::export]]
Rcpp::NumericVector hello_world3(int n) {
  Rcpp::NumericVector ret(n);
  for (int i = 0; i < n; ++i) {
    ret[i] = R::rcauchy(0, 1);
  }
  return ret;
}

// An Rcpp "sugar" version

// [[Rcpp::export]]
Rcpp::NumericVector hello_world4(int n) {
  return Rcpp::rcauchy(n, 0, 1);
}
