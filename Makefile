SOURCES = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard src/*/*/*.cc)
OBJECTS = $(SOURCES:.c=.o)
CFLAGSO = -std=c++17 -O2 -Wall -Wextra -pedantic

LDFLAGS := -lpthread -lSDL2 -lSDL2_image

swamp.out: $(OBJECTS)
	g++ $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean::
	rm swamp.out
