# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

CPP=g++
ROOT = $(shell pwd | sed 's/tectonics.cpp.*/tectonics.cpp/')
TEST = $(shell find ./ -type f -name '*_test.*pp')

FLAGS= -Wall -Werror -pedantic-errors -rdynamic -g 

all: $(shell find ./ -type f -name '*.*pp') ./Makefile
	rm -f test.cpp && \
	find ./ -type f -name '*_specialization.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	find ./ -type f -name '*_test.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -std=c++17 -o test.out test.cpp  $(FLAGS) \
	-I $(ROOT)/lib/ -I $(ROOT)/inc/ -I $(ROOT)/sketch/ && \
	chmod a+x test.out && \
	./test.out

clean:
	rm -f test.cpp test.out
