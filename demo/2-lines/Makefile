# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

CPP   = g++
OS    = $(shell uname -o | sed 's|[^/]*/||')
ROOT  = $(shell pwd | sed 's/tectonics.cpp.*/tectonics.cpp/')
TEST  = $(shell find ./ -type f -name '*_test.*pp')

LIBS-Linux    =-lglfw -lGL -lGLEW
LIBS-Msys     =-lglfw3 -lopengl32 -lgdi32 -lglew32
LIBS-Cygwin   =-lglfw3 -lopengl32 -lgdi32 -lglew32

FLAGS-Linux   =-Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE -D GLM_FORCE_PURE -D GLM_ENABLE_EXPERIMENTAL
FLAGS-Msys    =-Wall -Werror -pedantic-errors -g -D GLM_FORCE_SWIZZLE -D GLM_FORCE_PURE -D GLM_ENABLE_EXPERIMENTAL
FLAGS-Cygwin  =-Wall -Werror -pedantic-errors -g -D GLM_FORCE_SWIZZLE -D GLM_FORCE_PURE -D GLM_ENABLE_EXPERIMENTAL

all: $(TEST) ./Makefile
	rm -f test.cpp && \
	cp $(ROOT)/lib/glew/bin/Release/x64/glew32.dll . && \
	find ./ -type f -name '*_specialization.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	find ./ -type f -name '*_test.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -std=c++17 -o test.out test.cpp  $(FLAGS-$(OS)) \
	-I $(ROOT)/lib/ -I $(ROOT)/inc/ -I $(ROOT)/src/ $(LIBS-$(OS)) && \
	chmod a+x test.out && \
	./test.out

clean:
	rm -f test.cpp test.out
