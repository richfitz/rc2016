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

pi2 <- function(n) {
  x <- runif(n)
  y <- runif(n)
  4 * sum(x*x + y*y < 1) / n
}

dyn.load(paste0("pi", .Platform$dynlib.ext))
.Call("pi", 10000L, PACKAGE="pi")

library(microbenchmark)
microbenchmark(pi(100000), .Call("pi", 100000, PACKAGE="pi"))

microbenchmark(pi2(100000), .Call("pi", 100000, PACKAGE="pi"))
