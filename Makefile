_SOURCES := $(wildcard src/*.cpp)
_OBJECTS := $(patsubst %.cpp,%.o,$(_SOURCES))
_DEPS := $(wildcard src/*.h)
_PCHS := $(patsubst %.h,%.h.gch,$(_DEPS))


IDIR = src/
CPPFLAGS=-g -std=gnu++0x -I$(IDIR) $(shell fltk-config --cxxflags)

BUILD_DIR := bin/
PROGRAM_NAME := shadows

all: shadows

shadows: $(_OBJECTS)
	$(CXX) -std=c++0x $(_OBJECTS) -o $(BUILD_DIR)$(PROGRAM_NAME) $(shell fltk-config --use-gl --ldflags) -lGLEW -lSOIL

src/%.o: src/%.cpp
	echo "tst"
	$(CXX) -c -o $@ $< $(CPPFLAGS)

%.h.gch: %.h
	echo "hah"
	$(CXX) -c $< $(CPPFLAGS)

clean:
	rm $(_OBJECTS)
	rm $(BUILD_DIR)$(PROGRAM_NAME)
	rm $(_PCHS)