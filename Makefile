FILEASM = bin.s
EXEASM = basic_io.out
OBJASM = basic_io.o

CPP      = g++
CC       = gcc
SRCS	 = $(shell find src -name '*.cpp')
OBJ      = $(addprefix obj/,$(notdir $(SRCS:%.cpp=%.o))) 
LIBS     = 
INCS     = 
CXXINCS  = -I"include/"
BIN      = translator
CXXFLAGS = $(CXXINCS) -std=c++11 -g3 -ggdb3
CFLAGS   = $(INCS) -std=c11 -ggdb3
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

$(shell mkdir -p obj)

all: $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)

obj/%.o: src/%.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)

run:
	$ cd obj 
	$ ./sw

assembly:
	nasm -f elf -o $(OBJASM) $(FILEASM)
	ld -m elf_i386 -o $(EXEASM) $(OBJASM)
# clean:
# 	rm -f *.o *.out