SOURCES = $(wildcard src/*.cc) $(wildcard src/impl/*.cc) $(wildcard src/impl/*/*.cc) $(wildcard src/impl/*/*/*.cc)
OBJECTS = $(SOURCES:.c=.o)
#compilation includes the updater
SOURCES_UPDATER = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard src/*/*/*.cc) $(wildcard src/*/*/*/*.cc)
OBJECTS_UPDATER = $(SOURCES_UPDATER:.c=.o)
CFLAGSO = -std=c++17 -O2 -Wall -Wextra
CFLAGS = -std=c++17 -g

LDFLAGS := -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf
LDFLAGS_UPDATER := -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -lcurl

swamp.out: $(OBJECTS)
	g++ $(CFLAGSO) -o $@ $^ $(LDFLAGS)
macos: $(OBJECTS)
	g++ -headerpad_max_install_names $(CFLAGS) -DBUILD__MACOS__ -o SwampSurveyor $^ $(LDFLAGS)
macos-updater: $(OBJECTS_UPDATER)
	g++ -headerpad_max_install_names $(CFLAGS) -DBUILD__MACOS__ -DJACKHAYIO__UPDATER__ -o SwampSurveyor $^ $(LDFLAGS_UPDATER)
debug: $(OBJECTS)
	g++ $(CFLAGS) -o swamp.out $^ $(LDFLAGS)
clean::
	rm swamp.out
