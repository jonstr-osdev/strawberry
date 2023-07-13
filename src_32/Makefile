BOOT			=	asm/multiboot.asm
KERNEL			=	c/kernel.c
ISOFILE			=	build/stros.iso
ISO_VOLUME_NAME	=	STRAWBERRYOS
LINKER			=	linker.ld
KERNEL_OUT		=	build/stros.bin

SRC_DIR         = c
OBJ_DIR         = build
SRC_FILES       = $(shell find $(SRC_DIR) -name "*.c")


jorbis:
	mkdir -p $(OBJ_DIR)
	nasm -f elf32 $(BOOT) -o $(OBJ_DIR)/multiboot.o
	gcc -m32 -ffreestanding -c $(SRC_FILES)
	mv *.o build
	ld -m elf_i386 -T $(LINKER) -o $(KERNEL_OUT) build/*.o


all: 
	make build


build: 
	make clean
	mkdir -p build
	nasm -f elf32 ${BOOT} -o build/multiboot.o
	gcc -m32 -ffreestanding -c ${KERNEL} -o build/kernel.o
	ld -m elf_i386 -T ${LINKER} -o ${KERNEL_OUT} build/multiboot.o build/kernel.o


build-debug: 
	make clean
	mkdir -p build
	nasm -f elf32 ${BOOT} -o build/multiboot.o
	gcc -m32 -ffreestanding -c ${KERNEL} -o build/kernel.o -ggdb
	ld -m elf_i386 -T ${LINKER} -o ${KERNEL_OUT} build/multiboot.o build/kernel.o


run: 
	make build 
	qemu-system-i386 -kernel ${KERNEL_OUT} -monitor stdio


debug:
	make build-debug
	qemu-system-i386 -kernel ${KERNEL_OUT} -s -S &
	gdb -x .gdbinit


iso: 
	make build 
	mkdir -p build/iso/boot/grub
	cp grub.cfg build/iso/boot/grub
	cp ${KERNEL_OUT} build/iso/boot/grub
	grub-mkrescue -o build/stros.iso build/iso
	rm -rf build/iso


run-iso: 
	make iso
	qemu-system-i386 -cdrom ${ISOFILE} 


clean:
	rm -rf build
	rm -rf *.o