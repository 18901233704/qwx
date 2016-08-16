CC			= arm-linux-gnu-gcc
CXX		 	= arm-linux-gnu-g++
CFLAGS		= -g -Wall -std=gnu99
CXXFLAGS	= -g -Wall -std=c++11
CPATH		= -I/usr/arm-linux-gnu/include
CXXPATH		= -I/usr/arm-linux-gnu/include
LIBPATH		= -L/usr/arm-linux-gnu/lib
LIBS		= -nostdlib /usr/arm-linux-gnu/lib/crt1.o /usr/arm-linux-gnu/lib/crti.o /usr/arm-linux-gnu/lib/crtn.o -lc -ldl

all: hello hello-cpp

hello:
	$(CC) -o hello.o -c $(CFLAGS) $(CPATH) hello.c
	$(CC) -o hello hello.o $(LIBPATH) $(LIBS)

#hello-cpp:
#    $(CXX) -o hello-cpp.o -c $(CXXFLAGS) $(CXXPATH) hello.cpp
#    $(CXX) -o hello-cpp hello-cpp.o $(LIBPATH) $(LIBS)

clean: 
	rm -rf *.o hello hello-cpp
