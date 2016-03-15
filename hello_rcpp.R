devtools::load_all("testing")
hello_world()

microbenchmark::microbenchmark(
  hello_world2(1000L),
  hello_world3(1000L),
  hello_world4(1000L))
