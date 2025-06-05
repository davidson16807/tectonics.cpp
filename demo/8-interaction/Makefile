# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

CPP   = g++
OS    = $(shell uname -o | sed 's|[^/]*/||')
ROOT  = $(shell pwd | sed 's/tectonics.cpp.*/tectonics.cpp/')
TEST  = $(shell find ./ -type f -name '*_test.*pp')

OUT-Linux  = test.out
OUT-Msys   = test.exe
OUT-Cygwin = test.exe

LIBS-Linux    =-lglfw -lGL -lGLEW
LIBS-Msys     =-lglfw3 -lopengl32 -lgdi32 -lglew32
LIBS-Cygwin   =-lglfw3 -lopengl32 -lgdi32 -lglew32

FLAGS-Linux   =-fopenmp -march=native -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE
FLAGS-Msys    =-fopenmp -march=native -Wall -Werror -pedantic-errors -g -D GLM_FORCE_SWIZZLE
FLAGS-Cygwin  =-fopenmp -march=native -Wall -Werror -pedantic-errors -g -D GLM_FORCE_SWIZZLE

all: $(TEST) ./Makefile
	rm -f test.cpp && \
	cp $(ROOT)/lib/glew/bin/Release/x64/glew32.dll . && \
	find ./ -type f -name '*_specialization.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	find ./ -type f -name '*_test.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -O2 -std=c++20 -o $(OUT-$(OS)) test.cpp  $(FLAGS-$(OS)) \
	-I $(ROOT)/lib/ -I $(ROOT)/inc/ -I $(ROOT)/src/ $(LIBS-$(OS)) && \
	chmod a+x $(OUT-$(OS)) && \
	./$(OUT-$(OS))

clean:
	rm -f test.cpp $(OUT-$(OS))
