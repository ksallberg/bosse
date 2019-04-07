UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    comp := gcc
    flags := -L /usr/local/lib/ -lGLEW -lGLUT -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
endif
ifeq ($(UNAME_S), FreeBSD)
    comp := gcc
    flags := -L /usr/local/lib/ -lglfw -lGLUT -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -ldl
endif
ifeq ($(UNAME_S), Darwin)
    comp := clang
    flags := -lglfw -lGLUT -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
endif


build:
	$(comp) src/bosse.c $(flags) -o bin/bosse

clean:
	rm -f bin/bosse

run:
	./bin/bosse

mem:
	valgrind -v --log-file="mem.txt" --leak-check=yes ./a.out
