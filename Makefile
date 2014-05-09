CC := g++
LINK_FILES := -lGL -lGLU -lSDL -lSDL_image -lSDL_ttf -lv8 -Wl,-R/usr/lib/ -lpthread
export OBJ_HOME := $(realpath obj)
export INCL_HOME := $(realpath include)
export INCLUDES := -I/usr/include/GL/ -I/usr/include/SDL/ -I$(INCL_HOME)
export CFLAGS := -Wall -Wswitch-enum -Werror $(INCLUDES) -std=gnu++11 -g
CLIENT := prognet
SERVER := server_prognet

default: $(CLIENT) $(SERVER)

.PHONY: sources
sources: $(wildcard src/*) $(wildcard include/*)
	cd src && $(MAKE)

$(CLIENT): sources
	$(CC) client.cpp obj/*.o -o $(CLIENT) $(LINK_FILES) $(CFLAGS)

$(SERVER): sources
	$(CC) server.cpp obj/*.o -o $(SERVER) $(LINK_FILES) $(CFLAGS)

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f $(CLIENT)
