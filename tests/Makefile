TEST_FILE := main.test

OBJECTS := $(wildcard $(OBJ_HOME)/*.o)
OBJECTS_NO_MAIN := $(subst $(OBJ_HOME)/main.o,,$(OBJECTS))

.PHONY: run
run: $(TEST_FILE)
	

$(TEST_FILE): ../$(EXECUTABLE) $(TEST_LINK_FILES) $(wildcard *.cpp)
	$(CXX) $(CFLAGS) $(TEST_INCLUDE) $(wildcard *.cpp) -o $@ $(TEST_LINK_FILES) $(OBJECTS_NO_MAIN) $(LINK_FILES)

.PHONY: clean
clean:
	rm -f $(TEST_FILE)