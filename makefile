CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include
EXECUTABLE  := van

all: $(BIN)/$(EXECUTABLE)

run: clean all
	@echo "🚀..."
	@./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "🚧..."
	@$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹..."
	@-rm $(BIN)/*
