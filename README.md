# rc2016

This exists only to test if you have devtools / Rcpp and a compiler set up correctly.

```r
path <- tempfile()
dir.create(path)
devtools::with_lib(path, {
  devtools::install_github("richfitz/rc2016@install", force=TRUE)
  library(rc2016)
})
```

which should print

```
Everything looks good to me
```
