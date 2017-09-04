ifeq ($(CXX),)
CXX      := g++
endif
CXXFLAGS := -c -Wall -std=gnu++11
LDFLAGS  :=
SOURCES  := dxsyx/main.cpp dxsyx/dxsyx.cpp dxsyx/mk2syx.cpp
INCLUDES := dxsyx/dxsyx.h dxsyx/mk2syx.h
OBJECTS  := $(SOURCES:.cpp=.o)
OUT_DIR  := bin
EXE      := $(OUT_DIR)/dxsyx
MKDIR_P  := mkdir -p

.PHONY: directories test

all: directories $(SOURCES) $(EXE)

directories: $(OUT_DIR)

$(OUT_DIR):
	$(MKDIR_P) $(OUT_DIR)

$(EXE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

test:
	cd test && make

clean:
	rm $(OBJECTS) $(EXE) && cd test && make clean
