; GDT - Global Descriptor Table
gdt_start:
gdt_null:	; Null entry must be included first (error check)
	dd 0x0
	dd 0x0
gdt_code:	; Code segment descriptor
	dw 0xffff
	dw 0x0000
	db 0x00
	db 10011010b
	db 11001111b
	db 0x00
gdt_data:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00

gdt_end:		; Needed to calculate GDT size for inclusion in GDT descriptor

; GDT Descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; Define constants
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
