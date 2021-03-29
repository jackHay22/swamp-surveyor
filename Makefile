TARGET = swamp.out
SOURCES = $(wildcard src/*.cc) $(wildcard src/impl/*.cc) $(wildcard src/impl/*/*.cc) $(wildcard src/impl/*/*/*.cc)
BUILD_DIR = build
OBJECTS = $(SOURCES:.cc=.o)
BUILDOBJECTS := $(patsubst %,$(BUILD_DIR)/%,$(SOURCES:.cc=.o))
#compilation includes the updater
SOURCES_UPDATER = $(wildcard src/*.cc) $(wildcard src/*/*.cc) $(wildcard src/*/*/*.cc) $(wildcard src/*/*/*/*.cc)
OBJECTS_UPDATER = $(SOURCES_UPDATER:.cc=.o)
CFLAGSO = -std=c++17 -O2 -g -Wall
CFLAGS = -std=c++17 -g

LDFLAGS := -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf
LDFLAGS_UPDATER := -lpthread -lSDL2 -lSDL2_image -lSDL2_ttf -lcurl

.PHONY: all clean
all: $(TARGET)

%.o: %.cc
	mkdir -p $(BUILD_DIR)/$(dir $@)
	g++ $(CFLAGSO) -o $(BUILD_DIR)/$@ -c $^

$(TARGET): $(OBJECTS)
	g++ $(CFLAGSO) $(BUILDOBJECTS) -o $@ $(LDFLAGS)
macos: $(SOURCES)
	g++ -headerpad_max_install_names $(CFLAGS) -DBUILD__MACOS__ -o SwampSurveyor $^ $(LDFLAGS)
macos-updater: $(SOURCES_UPDATER)
	g++ -headerpad_max_install_names $(CFLAGS) -DBUILD__MACOS__ -DJACKHAYIO__UPDATER__ -o SwampSurveyor $^ $(LDFLAGS_UPDATER)
debug: $(SOURCES)
	g++ $(CFLAGS) -o swamp.out $^ $(LDFLAGS)
clean::
	rm -r build || true
	rm $(TARGET) || true
