OUTPUT_FORMAT(elf32-i386)
ENTRY(_mb_start)
SECTIONS
{
	. = 1M;
	.text BLOCK(4K) : ALIGN(4K)
	{
		code = .; _code = .; __code = .;
		*(.multiboot)
		*(.text)
		. = ALIGN(4K); 
	}
	.data : 
	{
		data = .; _data = .; __data = .; 
		*(.data)
		*(.rodata)
		. = ALIGN(4K); 
	}
	.bss : {
		bss = .; _bss = .; __bss = .;
        __bss_start = .;
        *(.bss)
        . = ALIGN(4);
        __bss_end = .;
    }

	end = .; _end = .; __end = .;
}
