TARGET=2k48

CXXFLAGS=-Wall -c

DEBUG?=0
ifeq (${DEBUG}, 1)
	CXXFLAGS+=-g -DDEBUG
else
	CXXFLAGS+=-O3
endif

DEAMON?=0
ifeq (${DEAMON}, 1)
	CXXFLAGS+=-DDEAMON
endif

all: ${TARGET}

clean:
	rm -f ${TARGET} *.o

deamon: d${TARGET}

${TARGET}: main.o io.o 2048.o
	${CXX} -o $@ $^ -lncurses

main.o: main.cpp io.h 2048.h
	${CXX} ${CXXFLAGS} -o $@ $<

2048.o: 2048.cpp 2048.h io.h
	${CXX} ${CXXFLAGS} -o $@ $<

io.o: io.cpp io.h 2048.h
	${CXX} ${CXXFLAGS} -o $@ $<

.PHONY: all clean
