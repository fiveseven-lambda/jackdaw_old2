target=main.cpp io.cpp load.cpp errmsg.cpp
header=io.h load.h errmsg.h
jackdaw: $(target) $(header)
	g++ $(target) -ojackdaw 
