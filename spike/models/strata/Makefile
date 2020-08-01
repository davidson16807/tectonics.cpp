# NOTE: We include Makefiles in subdirectories to preserve locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Recursive Makefiles are considered harmful!

CPP=g++
ROOT = ../../..
TEST = $(shell find ./ -type f -name '*_test.cpp')
BIN = $(shell find ./bin/ -type f -name '*.out')
OBJ = $(shell find ./obj/ -type f -name '*.o')

FLAGS= -Wall -Werror -pedantic-errors -rdynamic -g 

all: test.cpp ./Makefile
	rm -f test.cpp && \
	find ./ -type f -name '*_test.cpp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -std=c++17 -o test.out test.cpp  $(FLAGS) -I $(ROOT)/lib/ -I $(ROOT)/inc/ && \
	chmod a+x test.out && \
	./test.out

clean:
	rm -f $(OBJ) $(BIN) test.cpp
