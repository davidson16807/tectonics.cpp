CPP=g++
#BIN = $(shell find src/ -type f -name '*.out')
#OBJ = $(shell find src/ -type f -name '*.o')
#SRC = $(shell find src/ -type f -name '*.cpp')
INC = $(shell find inc/ -type f -name '*.hpp')
TEST = $(shell find test/ -type f -name '*.cpp')

all: $(BIN)
test: bin/driver.out
	chmod a+x bin/driver.out && bin/driver.out
clean:
	rm -f $(OBJ) $(BIN)

DEVFLAGS= -Wall -Werror -pedantic-errors -rdynamic -g 
PRODFLAGS= -Wall -Werror -pedantic-errors -O3

# spike: get at least something working
spike-strata: spike/spike_strata.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_strata.cpp  $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-rasters: spike/spike_rasters.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_rasters.cpp $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-layered-rasters: spike/spike_layered_rasters.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_layered_rasters.cpp $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-meshes: spike/spike_meshes.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/spike_meshes.cpp  $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/spike.out && time ./bin/spike.out
spike-genes: spike/models/genes/test/test_genes.cpp
	$(CPP) -std=c++17 -o bin/spike.out spike/models/genes/test/test_genes.cpp  $(DEVFLAGS) -I lib/ -I inc/ -I spike && chmod a+x bin/spike.out && time ./bin/spike.out

# test: get it working with confidence
test-layered-rasters: test/test_layered_rasters.cpp
	$(CPP) -std=c++17 -o bin/test.out test/test_layered_rasters.cpp $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/test.out && ./bin/test.out

# profile: get it working fast
profile-rasters: profile/profile_rasters.cpp
	$(CPP) -std=c++17 -o bin/profile.out profile/profile_rasters.cpp $(DEVFLAGS) -I lib/ -I inc/ && chmod a+x bin/profile.out && ./bin/profile.out

# demo: get it working in 3d
demo-gl: demo/demo_gl.cpp
	$(CPP) -std=c++17 -o bin/demo.out demo/demo_gl.cpp $(DEVFLAGS) -I lib/ -I inc/ -lglfw -lGL -lGLEW && chmod a+x bin/demo.out && ./bin/demo.out
demo-rasters: demo/demo_rasters.cpp
	$(CPP) -std=c++17 -o bin/demo.out demo/demo_rasters.cpp $(DEVFLAGS) -I lib/ -I inc/ -I src/ -lglfw -lGL -lGLEW && chmod a+x bin/demo.out && ./bin/demo.out
