##' @importFrom Rcpp evalCpp
##' @useDynLib rc2016
.onLoad <- function(...) {
  .Call("test_c", PACKAGE="rc2016")
  test_cpp()
  packageStartupMessage("Everything looks good to me")
  NULL
}
