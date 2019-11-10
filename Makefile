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

DEVFLAGS= -Wall -Werror -pedantic-errors -rdynamic -g 
PRODFLAGS= -Wall -Werror -pedantic-errors -O3

# spike: get at least something working
spike-strata: spike/spike_strata.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_strata.cpp  $(DEVFLAGS) -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-rasters: spike/spike_rasters.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_rasters.cpp $(DEVFLAGS) -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-meshes: spike/spike_meshes.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_meshes.cpp  $(DEVFLAGS) -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out

# test: get it working with confidence
test-many: test/test_many.cpp
	$(CPP) -std=c++17 -o bin/test.out test/test_many.cpp $(DEVFLAGS) -I inc/ && chmod a+x bin/test.out && ./bin/test.out
test-rasters: test/test_rasters.cpp
	$(CPP) -std=c++17 -o bin/test.out test/test_rasters.cpp $(DEVFLAGS) -I inc/ && chmod a+x bin/test.out && ./bin/test.out

# profile: get it working fast
profile-rasters: profile/profile_rasters.cpp
	$(CPP) -std=c++17 -o bin/profile.out profile/profile_rasters.cpp $(DEVFLAGS) -I inc/ && chmod a+x bin/profile.out && ./bin/profile.out
