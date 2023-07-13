; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "can we actually do shit the correct way?"
;  - https://wiki.osdev.org/Bare_Bones_with_NASM
;
; **************************************************************************************

[BITS 32]


_mb_ALIGN               equ  1 << 0                        ; align loaded modules on page boundaries
_mb_MEMINFO             equ  1 << 1                        ; provide memory map
_mb_FLAGS               equ  _mb_ALIGN | _mb_MEMINFO       ; this is the Multiboot 'flag' field
_mb_MAGIC               equ  0x1BADB002                    ; 'magic number' lets bootloader find the header
_mb_CHECKSUM            equ -(_mb_MAGIC + _mb_FLAGS)       ; checksum of above, to prove we are multiboot


__asm_screen_width_tty  equ 80


section .note.GNU-stack noalloc noexec nowrite progbits


section .multiboot
align 4
    dd _mb_MAGIC
    dd _mb_FLAGS
    dd _mb_CHECKSUM


section .bss
align 16
    _mb_stack_bottom:                                        ; stack allocation
    resb 16384
    _mb_stack_top:


section .text
    global _mb_start:function (_mb_start.mb_end - _mb_start)

    ; main lives here
    %include "asm/inc/extern.asm"
    %include "asm/inc/gdt.asm"

    _mb_start:
        lgdt [gdt_descriptor]
        jmp CODE_SEG:.setcs       ; Set CS to our 32-bit flat code selector
        .setcs:
        mov ax, DATA_SEG          ; Setup the segment registers with our flat data selector
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov esp, _mb_stack_top    ; set stack pointer
        cli                       ; Disable interrupts
        call main
    
    .mb_hang:
        hlt
        jmp .mb_hang
    .mb_end:
