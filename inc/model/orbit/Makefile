# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

CPP=g++
ROOT = $(shell pwd | sed 's/tectonics.cpp.*/tectonics.cpp/')
TEST = $(shell find ./ -type f -name '*_test.*pp')

# GLM_FORCE_SWIZZLE       support swizzling in glm
# GLM_FORCE_PURE          disable anonymous structs so we can build with ISO C++
# GLM_ENABLE_EXPERIMENTAL disable anonymous structs so we can build with ISO C++
FLAGS= -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE -D GLM_FORCE_PURE -D GLM_ENABLE_EXPERIMENTAL

all: $(TEST) ./Makefile
	rm -f test.cpp && \
	find ./ -type f -name '*_specialization.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	find ./ -type f -name '*_test.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -std=c++17 -o test.out test.cpp  $(FLAGS) \
	-I $(ROOT)/lib/ -I $(ROOT)/inc/ -I $(ROOT)/sketch/ && \
	chmod a+x test.out && \
	./test.out

clean:
	rm -f test.cpp test.out
