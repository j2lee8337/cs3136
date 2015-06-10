CC  = clang++
CXX = clang++
INCLUDES =
CFLAGS   = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)
LDFLAGS  = -g
LDLIBS   = -lncurses

EXECUTABLE = maze

$(EXECUTABLE): maze.o

.PHONY: clean
clean:
	rm -f *.o a.out core $(EXECUTABLE)

.PHONY: all
all: clean $(EXECUTABLE)