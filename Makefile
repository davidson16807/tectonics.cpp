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

test-many: test/test_many.cpp
	$(CPP) -std=c++17 -o bin/test.out test/test_many.cpp -Wall -Werror -pedantic-errors -I inc/ && chmod a+x bin/test.out && ./bin/test.out
test-rasters: test/test_rasters.cpp
	$(CPP) -std=c++17 -o bin/test.out test/test_rasters.cpp -Wall -Werror -pedantic-errors -I inc/ && chmod a+x bin/test.out && ./bin/test.out

spike-strata: spike/spike_strata.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_strata.cpp -Wall -Werror -pedantic-errors  -I inc/ && chmod a+x bin/spike.out && ./bin/spike.out
spike-rasters: spike/spike_rasters.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_rasters.cpp -Wall -Werror -pedantic-errors  -I inc/ && chmod a+x bin/spike.out && ./bin/spike.out
spike-meshes: spike/spike_meshes.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_meshes.cpp -Wall -Werror -pedantic-errors  -I inc/ && chmod a+x bin/spike.out && ./bin/spike.out
