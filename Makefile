CC := g++
export LINK_FILES := -lGL -lGLU -lSDL2 -lSDL2_net -lv8 -Wl,-R/usr/lib/ -lpthread
export TEST_LINK_FILES := -lgtest -lgtest_main
export OBJ_HOME := $(realpath obj)
export INCL_HOME := $(realpath include)
export INCLUDES := -I/usr/include/GL/ -I/usr/include/SDL2/ -I$(INCL_HOME)
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

.PHONY: tests
tests: sources
	cd tests && $(MAKE)
	tests/main.test

.PHONY: clean
clean: 
	rm -f obj/*.o
	rm -f $(CLIENT)
	rm -f tests/main.test
