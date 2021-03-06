// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// hello_world
double hello_world();
RcppExport SEXP testing_hello_world() {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    __result = Rcpp::wrap(hello_world());
    return __result;
END_RCPP
}
// hello_world2
std::vector<double> hello_world2(int n);
RcppExport SEXP testing_hello_world2(SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    __result = Rcpp::wrap(hello_world2(n));
    return __result;
END_RCPP
}
// hello_world3
Rcpp::NumericVector hello_world3(int n);
RcppExport SEXP testing_hello_world3(SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    __result = Rcpp::wrap(hello_world3(n));
    return __result;
END_RCPP
}
// hello_world4
Rcpp::NumericVector hello_world4(int n);
RcppExport SEXP testing_hello_world4(SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    __result = Rcpp::wrap(hello_world4(n));
    return __result;
END_RCPP
}
// pi_sample
double pi_sample(int n);
RcppExport SEXP testing_pi_sample(SEXP nSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    __result = Rcpp::wrap(pi_sample(n));
    return __result;
END_RCPP
}
// update_model
std::vector<double> update_model(std::vector<double> x, int nr, int nc);
RcppExport SEXP testing_update_model(SEXP xSEXP, SEXP nrSEXP, SEXP ncSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< std::vector<double> >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type nr(nrSEXP);
    Rcpp::traits::input_parameter< int >::type nc(ncSEXP);
    __result = Rcpp::wrap(update_model(x, nr, nc));
    return __result;
END_RCPP
}
// update_model2
std::vector<double> update_model2(int n, std::vector<double> x, int nr, int nc);
RcppExport SEXP testing_update_model2(SEXP nSEXP, SEXP xSEXP, SEXP nrSEXP, SEXP ncSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< std::vector<double> >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type nr(nrSEXP);
    Rcpp::traits::input_parameter< int >::type nc(ncSEXP);
    __result = Rcpp::wrap(update_model2(n, x, nr, nc));
    return __result;
END_RCPP
}
// update_model3
std::vector<double> update_model3(int n, std::vector<double> x, int nr, int nc);
RcppExport SEXP testing_update_model3(SEXP nSEXP, SEXP xSEXP, SEXP nrSEXP, SEXP ncSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< std::vector<double> >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type nr(nrSEXP);
    Rcpp::traits::input_parameter< int >::type nc(ncSEXP);
    __result = Rcpp::wrap(update_model3(n, x, nr, nc));
    return __result;
END_RCPP
}
// universe_create
SEXP universe_create(std::vector<double> x, int nr, int nc);
RcppExport SEXP testing_universe_create(SEXP xSEXP, SEXP nrSEXP, SEXP ncSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< std::vector<double> >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type nr(nrSEXP);
    Rcpp::traits::input_parameter< int >::type nc(ncSEXP);
    __result = Rcpp::wrap(universe_create(x, nr, nc));
    return __result;
END_RCPP
}
// universe_update
void universe_update(SEXP obj);
RcppExport SEXP testing_universe_update(SEXP objSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type obj(objSEXP);
    universe_update(obj);
    return R_NilValue;
END_RCPP
}
// universe_get
std::vector<double> universe_get(SEXP obj);
RcppExport SEXP testing_universe_get(SEXP objSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type obj(objSEXP);
    __result = Rcpp::wrap(universe_get(obj));
    return __result;
END_RCPP
}
