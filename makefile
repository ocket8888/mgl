# MGL Makefile - CYGWIN | LINUX

# Automatically generate variable name lists
CGRAPH_SOURCES = $(wildcard source/cpp/*.cpp) $(source/platform/min/glew.cpp)

# Convert the .cpp files to .o files
OBJGRAPH_SOURCES = $(CGRAPH_SOURCES:.cpp=.o)

# Query the freetype2 package config for the include directory
FREETYPE2_INCLUDE = $(shell pkg-config freetype2 --cflags)

# Linker parameters
ifeq ($(OS),Windows_NT)
	# 64 bit
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        MGL_PATH = C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/include/mgl
    endif

	#64 bit
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        MGL_PATH = C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/include/mgl
    else
	#32 bit
        MGL_PATH = C:/cygwin/usr/i686-w64-mingw32/sys-root/mingw/include/mgl
    endif

	LDFLAGS += -lopengl32 -lgdi32 -lmingw32 -lfreetype.dll -lOpenAL32.dll -lvorbisfile.dll
else
	MGL_PATH = /usr/include/mgl
	LDFLAGS += -lX11 -lGL -lfreetype -lopenal -lvorbisfile
endif

# Include directories
BENCH_SOURCES = -Ibench/math -Ibench/geom -Ibench/scene -Ibench/file

# Enable GS rendering
ifdef MGL_VB43
	CPPFLAGS += -DMGL_VB43
endif

# Compile parameters
CXXFLAGS += -s -std=c++14 -Wall -g -O3 -march=native -fPIC -fomit-frame-pointer -freciprocal-math -ffast-math --param max-inline-insns-auto=100 --param early-inlining-insns=200
EXTRA = source/platform/min/glew.cpp
EX1 = $(EXTRA) example/programs/ex1.cpp
EX2 = $(EXTRA) example/programs/ex2.cpp
EX3 = $(EXTRA) example/programs/ex3.cpp
EX4 = $(EXTRA) example/programs/ex4.cpp
EX5 = $(EXTRA) example/programs/ex5.cpp
EX6 = $(EXTRA) example/programs/ex6.cpp
EX7 = $(EXTRA) example/programs/ex7.cpp
EX8 = $(EXTRA) example/programs/ex8.cpp
EX9 = $(EXTRA) example/programs/ex9.cpp
EX10 = $(EXTRA) example/programs/ex10.cpp

# Override if MGL_DESTDIR specified
ifdef MGL_DESTDIR
	MGL_PATH = $(MGL_DESTDIR)/mgl
endif

ARFLAGS = -cvr


# Main source files
INCLUDES = -Isource/ -Itest/ $(FREETYPE2_INCLUDE)
SOURCES := $(wildcard source/*/min/*.cpp)
OBJECTS := $(SOURCES:cpp=o)

# Test source files
TEST_SOURCES := $(wildcard test/*/min/*.cpp)
TEST_OBJECTS :=	$(TEST_SOURCES:cpp=o)
TEST_LDFLAGS := -L. -lmgl $(LDFLAGS)

.PHONY: all install clean extra-clean tests


# main targets
all: libmgl.a libmgl.so

libmgl.so: $(OBJECTS)
	$(CXX) -std=c++14 $^ $(LDFLAGS) -shared -o $@

libmgl.a: $(OBJECTS)
	$(AR) $(ARFLAGS) libmgl.a $?

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<


# test targets
tests: all bin/al_test bin/gl_test

bin/al_test: test/al_test.o $(TEST_OBJECTS)
	$(CXX) -std=c++14 $^ $(TEST_LDFLAGS) -o $@ && $@

bin/gl_test: test/gl_test.o $(TEST_OBJECTS)
	$(CXX) -std=c++14 $< $(TEST_LDFLAGS) -o $@ && $@

bin/wl_test: test/wl_test.o $(TEST_OBJECTS)
	$(CXX) -std=c++14 $< $(TEST_LDFLAGS) -o $@ && $@


# cleaning targets
clean:
	$(RM) $(OBJECTS) $(TEST_OBJECTS) test/al_test.o test/gl_test.o test/wl_test.o

extra-clean: clean
	$(RM) libmgl.so libmgl.a bin/al_test bin/gl_test bin/wl_test

# All run targets
install:
	mkdir -p $(MGL_PATH)
	cp -r source/* $(MGL_PATH)
uninstall:
	$(RM) -rI $(MGL_PATH)

gl_test:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) -Itest $(PARAMS) $(TEST_GL) -o bin/gl_test $(LDFLAGS) 2> "gl_test.txt"
wl_test:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) -Itest $(WL_INCLUDE) $(PARAMS) $(TEST_WL) -o bin/wl_test $(LDFLAGS) 2> "wl_test.txt"

benchmarks:
	g++ $(LIB_SOURCES) $(BENCH_SOURCES) -Ibench $(PARAMS) bench/gl_bench.cpp -o bin/gl_bench $(LDFLAGS) 2> "gcc_bench.txt"
example1:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX1) -o bin/ex1 $(LDFLAGS) 2> "min_ex1.txt"
example2:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX2) -o bin/ex2 $(LDFLAGS) 2> "min_ex2.txt"
example3:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX3) -o bin/ex3 $(LDFLAGS) 2> "min_ex3.txt"
example4:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX4) -o bin/ex4 $(LDFLAGS) 2> "min_ex4.txt"
example5:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX5) -o bin/ex5 $(LDFLAGS) 2> "min_ex5.txt"
example6:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX6) -o bin/ex6 $(LDFLAGS) 2> "min_ex6.txt"
example7:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX7) -o bin/ex7 $(LDFLAGS) 2> "min_ex7.txt"
example8:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX8) -o bin/ex8 $(LDFLAGS) 2> "min_ex8.txt"
example9:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX9) -o bin/ex9 $(LDFLAGS) 2> "min_ex9.txt"
example10:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX10) -o bin/ex10 $(LDFLAGS) 2> "min_ex10.txt"
examples: example1 example2 example3 example4 example5 example6 example7 example8 example9 example10
