CC := g++
SOURCES := $(wildcard *.cpp)
OBJECTS := $(addprefix $(OBJ_HOME)/,$(SOURCES:.cpp=.o))

.PHONY: main
main: $(OBJECTS)
	

$(OBJ_HOME)/%.o: %.cpp $(wildcard $(INCL_HOME)/*)
	$(CC) -c $(CFLAGS) $< -o $@ -g



.PHONY: clean
clean:
	rm *.o