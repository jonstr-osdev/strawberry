# JON-STR 6-30-2023

all: assemble run-qemu-x86_64

assemble:
	nasm -f bin boot.asm -o boot.bin

run-qemu-i386:
	qemu-system-i386 -drive format=raw,file="boot.bin",index=0,if=floppy, -m 128M

run-qemu-x86_64:
	qemu-system-x86_64 -drive format=raw,file="boot.bin",index=0,if=floppy, -m 128M

iso:
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img seek=0 count=1 conv=notrunc
	mkdir -p iso
	cp floppy.img iso
	genisoimage -quiet -V 'strawberry' -input-charset iso8859-1 -o strawberry.iso -b floppy.img -hide floppy.img iso

clean:
	rm -f *.bin
