CPP = g++
CPPFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = tbendec

%.o: %.cpp
	$(CPP) -O3 -c $< -o $@ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) -lcrypto -lsqlite3 -pthread -larchive

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p ~/test/ -v encrypt test.tar.ct
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p . -v decrypt test.tar.ct
