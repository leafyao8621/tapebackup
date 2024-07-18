CPP = g++
CPPFLAGS = -Wall -Wextra -pedantic -Werror
SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = tbcli

%.o: %.cpp
	$(CPP) -O3 -c $< -o $@ $(CPPFLAGS)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) -lcrypto -lsqlite3 -pthread -larchive

.PHONY: clean valgrind
clean:
	@rm $(OBJ)
	@rm $(BIN)
valgrind: $(BIN)
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p ~/test/ -v write ~/dummy.img
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -p . -v read ~/dummy.img
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -v read ~/dummy.img
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -b "2024-07-01" -e "2024-08-01" report daily
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -b "2024-07-01" -e "2024-08-01" -f CSV report daily
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -b "2024-07-01" -e "2024-08-01" -x daily.txt report daily
	@valgrind -s --leak-check=full --show-leak-kinds=all ./$(BIN) -b "2024-07-01" -e "2024-08-01" -f CSV -x daily.csv report daily
