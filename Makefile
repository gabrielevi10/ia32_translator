FILE = basic_io.asm
EXE = basic_io.out
OBJ = basic_io.o

all:
	nasm -f elf -o $(OBJ) $(FILE)
	ld -m elf_i386 -o $(EXE) $(OBJ)
clean:
	rm -f *.o *.out