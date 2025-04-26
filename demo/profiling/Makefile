# NOTE: We include Makefiles in subdirectories to allow locality of unit tests,
# Please use make files within subdirectories only for unit tests, and do not use recursively
# Subdirectory makefiles are not bad by themself, but recursive Makefiles are considered harmful! 

CPP=g++
ROOT = $(shell pwd | sed 's/tectonics.cpp.*/tectonics.cpp/')
TEST = $(shell find ./ -type f -name '*_test.*pp')

FLAGS= -Wall -Werror -pedantic-errors -rdynamic -g -D GLM_FORCE_SWIZZLE

all: $(TEST) ./Makefile
	rm -f test.cpp test.out gmon.out && \
	find ./ -type f -name '*_specialization.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	find ./ -type f -name '*_test.*pp' -exec echo "#include \"{}\"" >> test.cpp \; && \
	$(CPP) -pg -O$(O) -std=c++20 -o test.out test.cpp  $(FLAGS) \
	-I $(ROOT)/lib/ -I $(ROOT)/inc/ -I $(ROOT)/src/ -I $(ROOT)/sketch/ \
	-lglfw -lGL -lGLEW && \
	chmod a+x test.out && \
	./test.out && \
	gprof test.out gmon.out > profiled-$(TAG)-O$(O).txt && \
	pythone3 replace.py beautify.tsv profiled-$(TAG)-O$(O).txt && \
	sed -i s/\[[0-9]+\]//g profiled-$(TAG)-O$(O).txt

clean:
	rm -f test.cpp test.out gmon.out
