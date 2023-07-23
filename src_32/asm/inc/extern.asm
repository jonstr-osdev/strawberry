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
extern keyboard_callback                        ; defined in keyboard.h


i_calc_offset:                                  ; (args: esp + 16 is row, esp + 12 is col, 
                                                ;        stack offsets increased by one word due to
                                                ;        return addr of i_calc_offset being pushed on sub call)
    mov eax, [esp + 4 + 4 + 4 + 4]            	; eax = row
    mov edx, __asm_screen_width_tty			    ; 80 (number of cols per row)
    mul edx								        ; now eax = row * 80
    add eax, [esp + 4 + 4 + 4]         	        ; now eax = row * 80 + col
    ; mov edx, 2						        ; * 2 because 2 bytes per char on screen
    ; mul edx
    shl eax, 1                                  ; multiply eax by 2
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
    call keyboard_callback
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


__asm_flush_gdt:    
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret


__asm_flush_idt:    
    mov eax, [esp+4]  ; Get the pointer to the IDT, passed as a parameter. 
    lidt [eax]        ; Load the IDT pointer.
    ret
