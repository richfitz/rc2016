# R & C / C++

---

## When (and why) to use R with C/C++

---

# DON'T

---

## Why not?

* Decrease in development speed
* Two languages to be good at
* Dealing with crashes in dynamically loaded code = not fun
* Profiling is harder
* Platform independence is harder

---

## Instead

* optimise and profile your code
  - `Rprof`
  - `system.time`
  - `microbenchmark`
* use existing library code
* check your algorithms

Using C will not magically make a bad algorithm faster

---

# But I *NEED* to!

---

## When is this the right move?

* Linking to external library
  - fftw, nlopt, eigen, etc
* *Proven* need for speed
* Reference semantics & copy avoidance

---

## Which interface?

* R + C: `.Call`, preferably in a package
* R + C++: `Rcpp`, in a package

(Do not use `.C` for _any_ new code)

* `inline` / `sourceCpp` - for quickly testing only

---

# Today

* Hello world in R & C, R & C++
* Rejection sampling estimation of pi
* Dealing with pointers in R and avoiding copies
* Programing with templates

I have no idea how much of this we can get through.

---

# Hello world in C

Write a program that returns one standard Cauchy random number

---

---

`hello.c`:

```c
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
SEXP hello_world() {
  GetRNGstate();
  double ret = rcauchy(0, 1);
  PutRNGstate();
  return ScalarReal(ret);
}
```

`hello.R`:

```r
dyn.load(paste0("hello", .Platform$dynlib.ext))
.Call("hello_world", PACKAGE="hello")
```

---

# Things to note

* No `main()` function
* `SEXP` - a polymorphic type
* `GetRNGstate` / `SetRNGstate`
* *Enormous* (but poorly documented) API

---

# What if we want to return a vector?

---

`hello2.c`:

```c
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
SEXP hello_world2(SEXP rn) {
  int n = INTEGER(rn)[0];
  SEXP ret = PROTECT(allocVector(REALSXP, n));
  double *r = REAL(ret);
  GetRNGstate();
  for (int i = 0; i < n; ++i) {
    r[i] = rcauchy(0, 1);
  }
  PutRNGstate();
  UNPROTECT(1);
  return ret;
}

```

---

`hello3.c`

```
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
SEXP hello_world3(SEXP rn) {
  if (LENGTH(rn) != 1) {
    error("Expected a scalar for 'n'");
  }
  rn = PROTECT(coerceVector(rn, INTSXP));
  int n = INTEGER(rn)[0];
  if (n == NA_INTEGER) {
    error("Expected non-NA 'n' (possibly caused by coersion?)");
  }
  SEXP ret = PROTECT(allocVector(REALSXP, n));
  double *r = REAL(ret);
  GetRNGstate();
  for (int i = 0; i < n; ++i) {
    r[i] = rcauchy(0, 1);
  }
  PutRNGstate();
  UNPROTECT(2);
  return ret;
}
```

---

# Things to note

* Memory allocation!
  - `allocVector()` - returns a `SEXP`
  - `R_alloc()` will give you auto-cleaned up memory
  - `Calloc()` and `Free()` for user-controlled memory
* Type coersion!
  - `REAL` / `INTEGER`
  - `coerceVector`
* Garbage collection!
  - `PROTECT` / `UNPROTECT`
  - works on a stack
  - simpler than it looks from all the shouting

---

# Hello world in C++

Using Rcpp

---

## Creating a package

In Rstudio:

* File: New Project...
* New Directory
* R Package
* Type: Package w/Rcpp
* Package name: `testing`
* Create package as subdirectory of: *pick a place*
* Create a git repository (naturally)
* File: New File: C++ File (delete the automatic guff)

---

---


`hello_rcpp.cpp`

```c++
#include <Rcpp.h>
// [[Rcpp::export]]
double hello_world() {
  return R::rcauchy(0, 1);
}
```

The R:

```r
hello_world()
```

---

```c++
// [[Rcpp::export]]
std::vector<double> hello_world2(int n) {
  std::vector<double> ret;
  ret.reserve(n);
  for (int i = 0; i < n; ++i) {
    ret.push_back(R::rcauchy(0, 1));
  }
  return ret;
}
```

---

```c++
// [[Rcpp::export]]
Rcpp::NumericVector hello_world3(int n) {
  Rcpp::NumericVector ret(n);
  for (int i = 0; i < n; ++i) {
    ret[i] = R::rcauchy(0, 1);
  }
  return ret;
}
```

---

```c++
// [[Rcpp::export]]
Rcpp::NumericVector hello_world4(int n) {
  return Rcpp::rcauchy(n, 0, 1);
}
```

---

# Type conversions

When passing arguments in and returning things
 - scalars vs vectors
 - bools, integers, doubles, *strings*
 - R lists?
 - R environments
 - R ***functions***
 - C pointers?
 - object lifecycles and garbage collection

---

# Other issues

* Random numbers (don't use C or C++'s base RNGs)
* Threading (R is single threaded)
* Parallel random numbers (this is a research area)
* Exceptions (very different to plain C/C++)
* Don't print with base functions (will not be portable)

Basically: R is running the show here; you need to use its API

---

# Rejection sampling

Estimating pi by rejection sampling

---


```r
pi <- function(n) {
  tot <- 1L
  for (i in seq_len(n)) {
    xy <- runif(2L)
    if (sum(xy * xy) <= 1) {
      tot <- tot + 1L
    }
  }
  4 * tot / n
}
```

---

---

``` c
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
  return ScalarReal(tot / (double) nn);
}
```

---

```r
library(microbenchmark)
microbenchmark(pi(100000), .Call("pi", 100000, PACKAGE="pi"))
# Unit: milliseconds
#                                expr        min
#                           pi(1e+05) 299.436097
#  .Call("pi", 1e+05, PACKAGE = "pi")   2.553606
```

---

<!-- check yourself before you wreck yourself -->

## but not always that impressive

``` r
pi2 <- function(n) {
  x <- runif(n)
  y <- runif(n)
  4 * sum(x*x + y*y < 1) / n
}
microbenchmark(pi2(100000), .Call("pi", 100000, PACKAGE="pi"))
# Unit: milliseconds
#                                expr      min
#                          pi2(1e+05) 6.393438
#  .Call("pi", 1e+05, PACKAGE = "pi") 2.553901
```

---

# Why not .C

* always copies all arguments
* limited/no support for non-atomic types
* no C++
* strings will make you cry

note that the R manuals conflict on preferring `.C` or `.Call`.  Use `.Call`.

---

# Strategies

* Use a package
  - seems like more work at first, but *way* simpler in long term
  - platform independent compilation
  - use on cluster
  - unit tests
  - shorter scripts
* Implement the _innermost_ code
  - once in C, stay in C: don't call back out to R
* Writing code that requires user-provided compiled functions is hard
  - optimisers, mcmc samplers, ODEs
  - consider templates instead
  - or keep the outermost code in R
* Test on >1 platform as soon as you can (especially if one of these platforms can be Windows)
* Big data?  Use pointers.

---

# Turning programs into libraries

* Find the `main` function
* pull apart the command processing functions and turn them into argument definitions.  Let Rcpp do the hard work of coersion for you.
* replace file output with wrapping data into R objects (again Rcpp)

---

# Resources

* [Writing R extensions](https://cran.r-project.org/doc/manuals/r-release/R-exts.html): the canonical manual.  Section 5 and 6 are what you want.  There is no list of functions that are the R API.  Ctrl-F is your friend :(
* `library(help=Rcpp)` - the vignettes.
  - Start with `introduction` and `attributes`.
  - Dip into `quickref` and `FAQ` as you need them
  - Some goodies in `sugar`, but optional
  - There is no good reference to methods of various Rcpp types.  The Rcpp authors will refer you to the Doxygen docs, but I've never made any sense of them.
* [Advanced R](http://adv-r.had.co.nz) has a chapter on Rcpp
