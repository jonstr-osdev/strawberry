; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "can we actually do shit the correct way?"
;  - https://wiki.osdev.org/Bare_Bones_with_NASM
;
; **************************************************************************************

[BITS 32]

global _mb_boot

extern code
extern bss
extern end

_mb_ALIGN               equ  1 << 0                        ; align loaded modules on page boundaries
_mb_MEMINFO             equ  1 << 1                        ; provide memory map
_mb_FLAGS               equ  _mb_ALIGN | _mb_MEMINFO       ; this is the Multiboot 'flag' field
_mb_MAGIC               equ  0x1BADB002                    ; 'magic number' lets bootloader find the header
_mb_CHECKSUM            equ -(_mb_MAGIC + _mb_FLAGS)       ; checksum of above, to prove we are multiboot


section .note.GNU-stack noalloc noexec nowrite progbits


section .multiboot
_mb_boot:
align 4
    dd _mb_MAGIC
    dd _mb_FLAGS
    dd _mb_CHECKSUM

    dd _mb_boot
    dd code
    dd bss
    dd end
    dd _mb_start

section .bss
__bss_start:
align 16
    _mb_stack_bottom:               ; stack allocation
    resb 16384
    _mb_stack_top:
__bss_end:


section .text
    global _mb_start:function (_mb_start.mb_end - _mb_start)

    ; main lives here
    %include "asm/inc/extern.asm"
    %include "asm/inc/gdt.asm"
    %include "asm/inc/int.asm"

    _mb_start:
        push ebx                    ; load multiboot header location

        ; we load the GDT in c now
;        lgdt [gdt_descriptor]
;        jmp CODE_SEG:.setcs         ; Set CS to our 32-bit flat code selector
;        .setcs:
;        mov ax, DATA_SEG            ; Setup the segment registers with our flat data selector
;        mov ds, ax
;        mov es, ax
;        mov fs, ax
;        mov gs, ax
;        mov ss, ax
;        mov esp, _mb_stack_top      ; set stack pointer

        cli                         ; Disable interrupts

        ; Clear .bss section (zero-initialized globals)
        mov edi, __bss_start
        mov ecx, __bss_end - __bss_start
        xor eax, eax
        rep stosb

        call main
    
    .mb_hang:
        hlt
        jmp .mb_hang
    .mb_end:
