dyn.load(paste0("hello", .Platform$dynlib.ext))
.Call("hello_world", PACKAGE="hello")

dyn.load(paste0("hello2", .Platform$dynlib.ext))
.Call("hello_world2", 10L, PACKAGE="hello2")
.Call("hello_world2", 10, PACKAGE="hello2")
.Call("hello_world2", integer(0), PACKAGE="hello2")

.Call("hello_world3", 10, PACKAGE="hello2")
.Call("hello_world3", c(10, 20), PACKAGE="hello2")
.Call("hello_world3", "a", PACKAGE="hello2")
