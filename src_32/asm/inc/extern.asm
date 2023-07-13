; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "If it can be pretty why not make it so???"
;
;
; **************************************************************************************


[BITS 32]

__asm_screen_width_tty  equ 80
__asm_screen_mem_addr   equ 0xb8000


global __asm_print_char_at
global __asm_print_char_at_color

global __asm_load_IDT
global __asm_enable_interrupts

global __asm_keyboard_handler

global __asm_io_port_in_b
global __asm_io_port_in_w

global __asm_io_port_out_b

global __asm_flush_gdt
global __asm_flush_idt

extern main                                     ; defined in kernel.c
extern handle_keyboard_interrupt                ; defined in kernel.c


i_calc_offset:                                  ; (args: esp + 16 is row, esp + 12 is col, 
                                                ;        stack offsets increased by one word due to
                                                ;        return addr of i_calc_offset being pushed on sub call)
    mov eax, [esp + 4 + 4 + 4 + 4]            	; eax = row
    mov edx, __asm_screen_width_tty			    ; 80 (number of cols per row)
    mul edx								        ; now eax = row * 80
    add eax, [esp + 4 + 4 + 4]         	        ; now eax = row * 80 + col
    mov edx, 2						            ; * 2 because 2 bytes per char on screen
    mul edx
    mov edx, __asm_screen_mem_addr		        ; vid mem start in edx
    add edx, eax	                            ; vid mem offset in edx
    ret


__asm_print_char_at:                            ; offset = col + (row * width)
    call i_calc_offset
    mov eax, [esp + 4] 		                    ; char c
    mov [edx], al
    ret
    

__asm_print_char_at_color:                      ; offset = col + (row * width)
    call i_calc_offset
    mov eax, [esp + 4] 		                    ; char c
    mov [edx], al
    mov eax, [esp + 4 + 4 + 4 + 4]              ; eax = color
    mov [edx + 1], al
    ret


__asm_load_IDT: ; try and change single register subs to use a different register and see the effect
    mov edx, [esp + 4]
    lidt [edx]
    ret


__asm_enable_interrupts:
    sti
    ret


__asm_keyboard_handler:
    pushad
    cld
    call handle_keyboard_interrupt
    popad
    iretd


__asm_io_port_in_b:
    mov edx, [esp + 4]
    in al, dx
    ret                                             ; whatever is in eax is returned to c?


__asm_io_port_in_w:
    mov edx, [esp + 4] ; Load port number from stack
    in ax, dx          ; Read word from port
    ret                ; Return (value is in eax)


__asm_io_port_out_b:
    mov edx, [esp + 4]
    mov eax, [esp + 4 + 4]
    out dx, al
    ret


__asm_flush_gdt:    ; not implemented
    ret


__asm_flush_idt:    ; not implemented
    ret