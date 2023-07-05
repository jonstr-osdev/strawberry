; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "we use LODSB because we are the cool kids"
;
; **************************************************************************************


[BITS 16]
[org 0x7c00]


_start:
	cld									; clear direction flag; si will be ++'d

	call _clear_screen_16r

	mov si, v_string2
	call _print_string_16r

	mov cx, 0x0007						
	mov dx, 0xA120						
	call _wait_16r

	call _clear_screen_16r

	call _enable_cursor_16r

	mov dl, 30							; row
	mov dh, 10							; column
	call _move_cursor_16r

	mov ah, 0x0e						; BIOS print char 	
	mov si, v_string					

.next_char:								; Jump here when there's another character
	lodsb								; Move the character at the address in SI into AL and inc si
	cmp al, 0x0
	je .done	
	int 0x10							; BIOS print char

	push ax                             ; preserve al
	xor cx, cx						
	mov dx, 0x7530												
	call _wait_16r
	pop ax

	jmp .next_char

.done:
	mov cx, 0x0007						
	mov dx, 0xA120						
	call _wait_16r

	call _clear_screen_16r

.init_prompt:
	mov al, '>'
	call _print_char_16r

.prompt:
	call _read_char_16r
	call _print_char_16r
	cmp al, 'V'
	je _vga_fill_test
	cmp al, 'X'
	jne .prompt

	call _clear_screen_16r
	jmp .init_prompt

	jmp $								; hang

	v_string db 'STRAWBERRY OS 0.0',0	; Put our string in memory
	v_string2 db 'TEST STRING',0		; Put our string in memory


_vga_fill_test:
    call _set_vga_mode_16r

    xor cx, cx
	push cx
.loop:
    mov ax,0A000h        
    mov es,ax
	pop cx	           
    mov di,cx
    mov al, 0x04 						; color red
    stosb     							; write AL to ES:DI and increment DI

    ; check if we've filled the screen
    inc cx
    cmp cx, (320 * 200)
    jnb _end
	push cx

	; wait
    xor cx, cx						
    mov dx, 0x1D4C												
    call _wait_16r

    jmp .loop
	
_end:
	jmp $

	%include "inc/subs_16r.asm"

	times 510-($-$$) db 0				; padding
	dw 0xAA55							; standard PC boot sig
