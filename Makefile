FLAGS=-std=c++11 -Wall -Wextra -Wpedantic
INCFLAGS=-I`root-config --incdir`
LDFLAGS=`root-config --libs`

.PHONY: all clean

all:
	g++ LRB1.C -o prog1 $(FLAGS) $(LDFLAGS) $(INCFLAGS)
	g++ LRB2.C -o prog2 $(FLAGS) $(LDFLAGS) $(INCFLAGS)

clean:
	rm -f prog1 prog2 *.d *.so *.pcm
