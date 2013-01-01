_SOURCES := $(wildcard src/*.cpp)
_OBJECTS := $(patsubst %.cpp,%.o,$(_SOURCES))
_DEPS := $(wildcard src/*.h)


IDIR = src/
CPPFLAGS=-g -std=gnu++0x -fpch-preprocess -I$(IDIR) $(shell fltk-config --cxxflags)

_PCH_IN := src/shadows-common.h
_PCH := $(patsubst %.h,%.h.gch,$(_PCH_IN))

BUILD_DIR := bin/
PROGRAM_NAME := shadows

all: shadows

shadows: $(_OBJECTS)
	$(CXX) -std=c++0x $(_OBJECTS) -o $(BUILD_DIR)$(PROGRAM_NAME) $(shell fltk-config --use-gl --ldflags) -lGLEW -lSOIL

src/%.o: src/%.cpp $(_DEPS) $(_PCH)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

$(_PCH): $(_PCH_IN) $(_DEPS)
	$(CXX) -c $< $(CPPFLAGS)

clean:
	rm $(_OBJECTS)
	rm $(BUILD_DIR)$(PROGRAM_NAME)
	rm $(_PCH)