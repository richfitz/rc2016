set.seed(1)

nr <- 10L
nc <- 11L
m <- sample(3, nr * nc, replace=TRUE)
plt <- function(m, nr, nc) {
  op <- par(mar=rep(0, 4))
  on.exit(par(op))
  image(matrix(m, nr, nc), axes=FALSE, bty="n")
}

sv <- function(m, nr, nc, filename) {
  png(filename)
  on.exit(dev.off())
  plt(m, nr, nc)
}

devtools::load_all("testing")
dir.create("output")

sv(m, nr, nc, "output/start.png")

for (i in 1:100) {
  m <- update_model(m, nr, nc)
}
sv(m, nr, nc, "output/m-100.png")
for (i in 1:900) {
  m <- update_model(m, nr, nc)
}
sv(m, nr, nc, "output/m-1000.png")

## This is due to copying in two places:
microbenchmark::microbenchmark(
  update_model(m, nr, nc),
  update_model2(1L, m, nr, nc),
  update_model2(10L, m, nr, nc),
  update_model2(100L, m, nr, nc),
  update_model2(1000L, m, nr, nc),
  update_model3(1L, m, nr, nc),
  update_model3(10L, m, nr, nc),
  update_model3(100L, m, nr, nc),
  update_model3(1000L, m, nr, nc))

nr <- 1000
nc <- 1000
m <- sample(3, nr * nc, replace=TRUE)
ptr <- universe_create(m, nr, nc)

nr <- 10
nc <- 10
m <- sample(3, nr * nc, replace=TRUE)
ptr <- universe_create(m, nr, nc)
for (i in 1:100) {
  universe_update(ptr)
}

nr <- 200
nc <- 200
m <- sample(3, nr * nc, replace=TRUE)
ptr <- universe_create(m, nr, nc)
for (i in 1:1000000) {
  universe_update(ptr)
}
m <- universe_get(ptr)
plt(m, nr, nc)

## C version

dyn.load(paste0("simulation", .Platform$dynlib.ext))
nr <- 10L
nc <- 10L
m <- as.integer(sample(3, nr * nc, replace=TRUE))
ptr <- .Call("universe_create", m, nr, nc)
for (i in 1:1000) {
  .Call("universe_update", ptr)
}
m <- .Call("universe_get", ptr, PACKAGE="simulation")
