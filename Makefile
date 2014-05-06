CC := g++
LINK_FILES := -lGL -lGLU -lSDL -lSDL_image -lSDL_ttf -lv8 -Wl,-R/usr/lib/ -lpthread
export OBJ_HOME := $(realpath obj)
export INCL_HOME := $(realpath include)
EXECUTABLE := prognet

$(EXECUTABLE): $(wildcard src/*) $(wildcard include/*)
	cd src && $(MAKE)
	$(CC) -Wall -std=gnu++11 obj/*.o -o $(EXECUTABLE) $(LINK_FILES)

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f $(EXECUTABLE)
