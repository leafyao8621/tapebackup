CPP = g++
CPPFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = tbcli

%.o: %.cpp
	$(CPP) -g -c $< -o $@ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN)

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) ~/dummy.img
