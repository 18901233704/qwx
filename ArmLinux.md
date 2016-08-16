CC			= arm-linux-gnu-gcc
CXX		 	= arm-linux-gnu-cpp
CFLAGS		= -g -fno-short-enums
CXXFLAGS	= -g -fno-short-enums
CPATH		= -I/usr/arm-linux-gnu/include
CXXPATH		= -I/usr/arm-linux-gnu/include
LIBPATH		= -Wl,-rpath-link=/usr/arm-linux-gnu/lib -L/usr/arm-linux-gnu/lib
LIBS		= -nostdlib -lgcc -lg -lc -lm

all: hello

hello:
	$(CC) -o hello.o -c $(CFLAGS) $(CPATH) hello.c
	$(CC) -o hello hello.o $(LIBPATH) $(LIBS)

clean: 
	rm -rf *.o hello
