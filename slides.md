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
* Want a programmable interface to a model
* You are thinking of doing I/O in C

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
* Programing with templates (if we get time)

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

* Error handling!
  - `error()`
* Memory allocation!
  - `allocVector()` - returns a `SEXP`
  - `R_alloc()` will give you auto-cleaned up memory
  - `Calloc()` and `Free()` for user-controlled memory

---

# Things to note

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

# Other issues, Rcpp

* Don't use `error`, use `Rcpp::stop`, or risk crashes
* Don't use `std::cout` / `std::cerr`, use `Rcpp::Rcout` / `Rcpp::Rcerr`
* Rcpp must be compiled with same compiler as packages that use it!

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

# Handling pointers and avoiding copies

---

```r
nr <- 10L
nc <- 11L
m <- sample(3, nr * nc, replace=TRUE)
plt <- function(m, nr, nc) {
  op <- par(mar=rep(0, 4))
  on.exit(par(op))
  image(matrix(m, nr, nc), axes=FALSE, bty="n")
}
plt(m, nr, nc)
```

---

![](output/start.png)

---

```c++
#include <Rcpp.h>
int clamp(int n, int max) {
  return n < 0 ? max - 1 : (n < max ? n : 0);
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
```

---

```r
for (i in 1:100) {
  m <- update_model(m, nr, nc)
}
plt(m, nr, nc)
```

---

![](output/m-100.png)

---

![](output/m-1000.png)

---

# Copies can be costly

* copies between R and C/C++
  - treat all `SEXP` and `Rcpp::` arguments as _read only_
  - cost goes up with number of args, size of objects
* copies within C/C++
  - copy elision can help
  - be careful with references and pointers

---

```c++
void update_model_ref(std::vector<int>& x, int nr, int nc) {
  int i = unif_rand() * nr, j = unif_rand() * nc;
  int t = x[i + j * nr];
  x[clamp(i - 1, nr) + j * nr] = t;
  x[clamp(i + 1, nr) + j * nr] = t;
  x[i + clamp(j - 1, nc) * nr] = t;
  x[i + clamp(j + 1, nc) * nr] = t;
}
```

---

```c++
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
```

---

```plain
Unit: microseconds
                            expr     min
         update_model(m, nr, nc)   3.225
    update_model2(1L, m, nr, nc)   3.707
   update_model2(10L, m, nr, nc)   6.360
  update_model2(100L, m, nr, nc)  32.852
 update_model2(1000L, m, nr, nc) 299.275
    update_model3(1L, m, nr, nc)   3.458
   update_model3(10L, m, nr, nc)   3.689
  update_model3(100L, m, nr, nc)   6.517
 update_model3(1000L, m, nr, nc)  37.147
```

---

## For big data, use pointers

```c++
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
```

---

```c++
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
```

---

```r
nr <- 1000
nc <- 1000
m <- sample(3, nr * nc, replace=TRUE)
ptr <- universe_create(m, nr, nc)
ptr
# <pointer: 0x7fd200f8a660>
```

---

```
microbenchmark::microbenchmark(
  universe_update(ptr),
  update_model(m, nr, nc),
  update_model2(1L, m, nr, nc))
# Unit: microseconds
#                          expr       min
#          universe_update(ptr)     3.313
#       update_model(m, nr, nc) 12444.179
#  update_model2(1L, m, nr, nc) 18715.152
```

---

```

nr <- 200
nc <- 200
m <- sample(3, nr * nc, replace=TRUE)
ptr <- universe_create(m, nr, nc)
for (i in 1:1000000) {
  universe_update(ptr)
}
m <- universe_get(ptr)
plt(m, nr, nc)
```

---

![](output/m-big.png)

---

## In C

This can be done, but it's a bit nastier (see `simulation.c`)

---

## Things to remember

* _Any_ data can be in the pointer
  - raw bytes, database handles, whatever
* Cannot be serialised
  - affects `mclapply`, `saveRDS`
* `NULL` pointer checking becomes important
* Clean up after yourself (finalisers are important)

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
  - seems like more work at first, *way* simpler in long term
  - platform independent compilation
  - use on cluster
  - unit tests
  - shorter scripts
* Test on >1 platform as soon as you can (especially if one of these platforms can be Windows)

---

# Strategies

* Implement the _innermost_ code
  - once in C, stay in C: don't call back out to R
* Writing code that requires user-provided compiled functions is hard
  - optimisers, mcmc samplers, ODEs
  - consider templates instead
  - or keep the outermost code in R

---

# Strategies

* Prototype in R, then translate
  - Write some tests
  - Implement same function
  - Compare output

---

# Turning programs into libraries

* Find the `main` function
* pull apart the command processing functions and turn them into argument definitions.  Let Rcpp do the hard work of coersion for you.
* replace file output with wrapping data into R objects (again Rcpp)

---

# Resources

* [Writing R extensions](https://cran.r-project.org/doc/manuals/r-release/R-exts.html): the canonical manual.  Section 5 and 6 are what you want.  There is no list of functions that are the R API.  Ctrl-F is your friend :(
* Hadley Wickham is writing new docs for the R/C API at https://github.com/hadley/r-internals

---

# Resources: Rcpp

* For Rcpp: `library(help=Rcpp)` - the vignettes.
  - Start with `introduction` and `attributes`.
  - Dip into `quickref` and `FAQ` as you need them
  - Some goodies in `sugar`, but optional
  - There is no good reference to methods of various Rcpp types.  There is [automatically generated reference documentation](http://dirk.eddelbuettel.com/code/rcpp/html/index.html) but I don't personally find it very useful.
  - there is a [book](http://www.rcpp.org/book) though it is now 3 years behind the package
* Hadley Wickham's free online book "[Advanced R](http://adv-r.had.co.nz)" has a [chapter]((http://adv-r.had.co.nz/Rcpp.html) on Rcpp, and another chapter on the [R/C interface](http://adv-r.had.co.nz/C-interface.html)
