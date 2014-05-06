CC := g++
INCLUDES := -I/usr/include/GL/ -I/usr/include/SDL/ -I$(INCL_HOME)
CFLAGS := -Wall -Wswitch-enum -Werror -c $(INCLUDES) -std=gnu++11 -g
SOURCES := $(wildcard *.cpp)
OBJECTS := $(addprefix $(OBJ_HOME)/,$(SOURCES:.cpp=.o))

.PHONY: main
main: $(OBJECTS)
	

$(OBJ_HOME)/%.o: %.cpp $(wildcard $(INCL_HOME)/*)
	$(CC) $(CFLAGS) $< -o $@ -g



.PHONY: clean
clean:
	rm *.o