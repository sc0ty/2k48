TARGET=2k48
SOURCES=main.cpp io.cpp 2048.cpp render.cpp
HEADERS=io.h 2048.h render.h
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS=-Wall -c

DEBUG?=0
ifeq ($(DEBUG), 1)
	CXXFLAGS+=-g -DDEBUG
else
	CXXFLAGS+=-O3
endif

DEAMON?=0
ifeq ($(DEAMON), 1)
	CXXFLAGS+=-DDEAMON
endif

all: $(TARGET)

clean:
	rm -f $(TARGET) *.o

$(TARGET): main.o io.o 2048.o render.o
	$(CXX) -o $@ $^ -lncurses

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: all clean

