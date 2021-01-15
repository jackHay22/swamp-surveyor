SOURCES = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard src/*/*/*.cc)
OBJECTS = $(SOURCES:.c=.o)
CFLAGSO = -std=c++17 -O2 -Wall -Wextra -pedantic
CFLAGS = -std=c++17 -g

LDFLAGS := -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf

swamp.out: $(OBJECTS)
	g++ $(CFLAGSO) -o $@ $^ $(LDFLAGS)
debug: $(OBJECTS)
	g++ $(CFLAGS) -o swamp.out $^ $(LDFLAGS)
clean::
	rm swamp.out
