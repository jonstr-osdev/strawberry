BOOT			=	asm/multiboot.asm
KERNEL			=	c/kernel.c
ISOFILE			=	build/stros.iso
ISO_VOLUME_NAME	=	STRAWBERRYOS
LINKER			=	linker.ld
KERNEL_OUT		=	build/stros.bin


all: 
	make build


build: 
	make clean
	mkdir -p build
	nasm -f elf32 ${BOOT} -o build/multiboot.o
	gcc -m32 -ffreestanding -c ${KERNEL} -o build/kernel.o
	ld -m elf_i386 -T ${LINKER} -o ${KERNEL_OUT} build/multiboot.o build/kernel.o


build: 
	make clean
	mkdir -p build
	nasm -f elf32 ${BOOT} -o build/multiboot.o
	gcc -m32 -ffreestanding -c ${KERNEL} -o build/kernel.o -ggdb
	ld -m elf_i386 -T ${LINKER} -o ${KERNEL_OUT} build/multiboot.o build/kernel.o


run: 
	make build 
	qemu-system-i386 -kernel ${KERNEL_OUT} -monitor stdio


run-debug:
	qemu-system-i386 -kernel ${KERNEL_OUT} -s S &


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