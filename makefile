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

	LINKER = -lopengl32 -lgdi32 -lmingw32 -lfreetype.dll -lOpenAL32.dll -lvorbisfile.dll
else
	MGL_PATH = /usr/include/mgl
	LINKER = -lX11 -lGL -lfreetype -lopenal -lvorbisfile
endif

# Include directories
INCLUDES = -Isource $(FREETYPE2_INCLUDE)
TEST_SOURCES = -Itest/file -Itest/geom -Itest/math -Itest/platform -Itest/renderer -Itest/scene -Itest/sound
BENCH_SOURCES = -Ibench/math -Ibench/geom -Ibench/scene -Ibench/file

# Enable GS rendering
ifdef MGL_VB43
	CPPFLAGS += -DMGL_VB43
endif

# Compile parameters
CXXFLAGS += -s -std=c++14 -Wall -O3 -march=native -fomit-frame-pointer -freciprocal-math -ffast-math --param max-inline-insns-auto=100 --param early-inlining-insns=200
EXTRA = source/platform/min/glew.cpp
WL_INCLUDE = -DGLEW_STATIC
TEST_AL = test/al_test.cpp
TEST_GL = test/gl_test.cpp
TEST_WL = $(EXTRA) test/wl_test.cpp
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

ARFLAGS = -cvq

LIBS = source/file/libmgl.file.a source/geom/libmgl.geom.a source/math/libmgl.math.a source/platform/libmgl.platform.a source/renderer/libmgl.renderer.a source/scene/libmgl.scene.a source/sound/libmgl.sound.a
FILE_SOURCES := $(addsuffix .o,$(basename $(wildcard source/file/min/*.cpp)))
GEOM_SOURCES := $(addsuffix .o,$(basename $(wildcard source/geom/min/*.cpp)))
MATH_SOURCES := $(addsuffix .o,$(basename $(wildcard source/math/min/*.cpp)))
PLAT_SOURCES := $(addsuffix .o,$(basename $(wildcard source/platform/min/*.cpp)))
REND_SOURCES := $(addsuffix .o,$(basename $(wildcard source/renderer/min/*.cpp)))
SCNE_SOURCES := $(addsuffix .o,$(basename $(wildcard source/scene/min/*.cpp)))
SOUN_SOURCES := $(addsuffix .o,$(basename $(wildcard source/sound/min/*.cpp)))

# For ease of cleaning
ALL_OBJECTS = $(FILE_SOURCES) $(GEOM_SOURCES) $(MATH_SOURCES) $(PLAT_SOURCES) $(REND_SOURCES) $(SCNE_SOURCES) $(SOUN_SOURCES)

# $(AR) $(ARFLAGS)

.PHONY: all install clean

all: $(LIBS)

source/file/libmgl.file.a: $(FILE_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/geom/libmgl.geom.a: $(GEOM_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/math/libmgl.math.a: $(MATH_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/platform/libmgl.platform.a: $(PLAT_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/renderer/libmgl.renderer.a: $(REND_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/scene/libmgl.scene.a: $(SCNE_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

source/sound/libmgl.sound.a: $(SOUN_SOURCES)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	$(RM) $(ALL_OBJECTS)

# Default run target
default: tests benchmarks examples

# All run targets
install:
	mkdir -p $(MGL_PATH)
	cp -r source/* $(MGL_PATH)
uninstall:
	$(RM) -rI $(MGL_PATH)
lib: $(OBJGRAPH_SOURCES)
	ar rvs bin/libmin.a $(OBJGRAPH_SOURCES)
al_test:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) -Itest $(PARAMS) $(TEST_AL) -o bin/al_test $(LINKER) 2> "al_test.txt"
gl_test:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) -Itest $(PARAMS) $(TEST_GL) -o bin/gl_test $(LINKER) 2> "gl_test.txt"
wl_test:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) -Itest $(WL_INCLUDE) $(PARAMS) $(TEST_WL) -o bin/wl_test $(LINKER) 2> "wl_test.txt"
tests: al_test gl_test wl_test

benchmarks:
	g++ $(LIB_SOURCES) $(BENCH_SOURCES) -Ibench $(PARAMS) bench/gl_bench.cpp -o bin/gl_bench $(LINKER) 2> "gcc_bench.txt"
example1:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX1) -o bin/ex1 $(LINKER) 2> "min_ex1.txt"
example2:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX2) -o bin/ex2 $(LINKER) 2> "min_ex2.txt"
example3:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX3) -o bin/ex3 $(LINKER) 2> "min_ex3.txt"
example4:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX4) -o bin/ex4 $(LINKER) 2> "min_ex4.txt"
example5:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX5) -o bin/ex5 $(LINKER) 2> "min_ex5.txt"
example6:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX6) -o bin/ex6 $(LINKER) 2> "min_ex6.txt"
example7:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX7) -o bin/ex7 $(LINKER) 2> "min_ex7.txt"
example8:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX8) -o bin/ex8 $(LINKER) 2> "min_ex8.txt"
example9:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX9) -o bin/ex9 $(LINKER) 2> "min_ex9.txt"
example10:
	g++ $(LIB_SOURCES) $(TEST_SOURCES) $(WL_INCLUDE) $(PARAMS) $(EX10) -o bin/ex10 $(LINKER) 2> "min_ex10.txt"
examples: example1 example2 example3 example4 example5 example6 example7 example8 example9 example10
