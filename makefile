CXX                 := g++
CXX_FLAGS           := -Wall -Wextra -pedantic -std=c++17 -O2 -s
CXX_DEBUG_FLAGS     := -Wall -Wextra -pedantic -std=c++17 -O0 -ggdb3

BIN                 := bin
SRC                 := src
INCLUDE             := include

EXECUTABLE          := analyzer
DEBUG_EXECUTABLE    := debug_analyzer

.PHONY: all
all: $(BIN)/$(EXECUTABLE)

.PHONY: run
run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

.PHONY: debug
debug:	debug_clean $(BIN)/$(DEBUG_EXECUTABLE)

$(BIN)/$(DEBUG_EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_DEBUG_FLAGS) -I$(INCLUDE) $^ -o $@

.PHONY: clean
clean:
	@-rm -f $(BIN)/$(EXECUTABLE)

.PHONY: debug_clean
debug_clean:
	@-rm -f $(BIN)/$(DEBUG_EXECUTABLE)
