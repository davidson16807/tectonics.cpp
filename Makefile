CPP=g++
BIN = $(shell find src/ -type f -name '*.out')
OBJ = $(shell find src/ -type f -name '*.o')
SRC = $(shell find src/ -type f -name '*.cpp')
INC = $(shell find inc/ -type f -name '*.hpp')
TEST = $(shell find test/ -type f -name '*.cpp')

all: $(BIN)
test: bin/driver.out
	chmod a+x bin/driver.out && bin/driver.out
clean:
	rm -f $(OBJ) $(BIN)

bin/driver.out: $(TEST)
	$(CPP) -std=c++17 -o bin/driver.out test/driver.cpp -I inc/

test-strata: spike/test_strata.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/test_strata.cpp -I inc/ && chmod a+x bin/spike.out && ./bin/spike.out