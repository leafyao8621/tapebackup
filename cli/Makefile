CPP = g++
CPPFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = tbcli

%.o: %.cpp
	$(CPP) -g -c $< -o $@ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) -lcrypto -lsqlite3 -pthread -ltar

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p ~/editor_dot_files -v write ~/dummy.img
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p . -v read ~/dummy.img
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -v read ~/dummy.img
