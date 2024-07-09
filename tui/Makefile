CPP = g++
CPPFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = tbtui

%.o: %.cpp
	$(CPP) -g -c $< -o $@ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) -lcrypto -lsqlite3 -pthread -lncurses -lmenu -ltar

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) ~/dummy.img
