; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "If it can be pretty why not make it so???"
;
;
; **************************************************************************************


[BITS 32]

__asm_screen_width_tty  equ 80


global __asm_print_char_at
global __asm_print_char_at_color


i_calc_offset:                                  ; (args: esp + 16 is row, esp + 12 is col, 
                                                ;        stack offsets increased by one word due to
                                                ;        return addr of i_calc_offset being pushed on sub call)
    mov eax, [esp + 4 + 4 + 4 + 4]            	; eax = row
    mov edx, __asm_screen_width_tty			    ; 80 (number of cols per row)
    mul edx								        ; now eax = row * 80
    add eax, [esp + 4 + 4 + 4]         	        ; now eax = row * 80 + col
    mov edx, 2						            ; * 2 because 2 bytes per char on screen
    mul edx
    mov edx, 0xb8000			                ; vid mem start in edx
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