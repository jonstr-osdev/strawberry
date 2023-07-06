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


; section .note.GNU-stack noalloc noexec nowrite progbits


section .multiboot
align 4
    dd _mb_MAGIC
    dd _mb_FLAGS
    dd _mb_CHECKSUM


section .bss
; align 16
    _mb_stack_bottom:
    ; resb 16384
    resb 8192
    _mb_stack_top:


section .text
    global _mb_start ;:function (_mb_start.mb_end - _mb_start)

    global __asm_print_char_at
    global __asm_print_char_at_color

    __asm_print_char_at:                            ; offset = col + (row * width)
        mov eax, [esp + 4 + 4 + 4]            		; eax = row
        mov edx, 80						            ; 80 (number of cols per row)
        mul edx								        ; now eax = row * 80
        add eax, [esp + 4 + 4] 	                    ; now eax = row * 80 + col
        mov edx, 2						            ; * 2 because 2 bytes per char on screen
        mul edx
        mov edx, 0xb8000			                ; vid mem start in edx
        add edx, eax					            ; Add our calculated offset
        mov eax, [esp + 4] 		                    ; char c
        mov [edx], al
        ret
        

    __asm_print_char_at_color:                            ; offset = col + (row * width)
        mov eax, [esp + 4 + 4 + 4]            		; eax = row
        mov edx, 80						            ; 80 (number of cols per row)
        mul edx								        ; now eax = row * 80
        add eax, [esp + 4 + 4] 	                    ; now eax = row * 80 + col
        mov edx, 2						            ; * 2 because 2 bytes per char on screen
        mul edx
        mov edx, 0xb8000			                ; vid mem start in edx
        add edx, eax					            ; Add our calculated offset
        mov eax, [esp + 4] 		                    ; char c
        mov [edx], al
        mov eax, [esp + 4 + 4 + 4 + 4]              ; eax = color
        mov [edx + 1], al
        ret

    _mb_start:
        mov esp, _mb_stack_top                  ; setup stack
        cli                                     ; disable interrupts 

        extern main                             ; defined in kernel.c
        call main
    
    .mb_hang:
        hlt
        jmp .mb_hang
    .mb_end:
