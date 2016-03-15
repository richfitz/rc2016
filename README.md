# rc2016

This exists only to test if you have devtools / Rcpp and a compiler set up correctly.

```r
devtools::dev_mode(TRUE, tempfile())
devtools::install_github("richfitz/rc2016@install", force=TRUE)
devtools::dev_mode(FALSE)
```
