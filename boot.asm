; **************************************************************************************
;
; JON-STR 6-30-2023
;  - "we use LODSB because we are the cool kids"
;
; **************************************************************************************

BITS 16
[org 0x7c00]

_start:
	cld									; clear direction flag; si will be ++'d

	call _clear_screen_16r

	call _enable_cursor_16r

	mov dl, 34							; row
	mov dh, 12							; column
	call _move_cursor_16r

	mov ah, 0x0e						; BIOS print char 	
	mov si, v_string					

.next_char:								; Jump here when there's another character
	lodsb								; Move the character at the address in SI into AL and inc si
	int 0x10							; BIOS print char

	push ax
	mov cx, 0x0007						
	mov dx, 0xA120						; 0x0007 A120 is 500,000 microseconds								; preserve al
	call _wait_16r
	pop ax

	cmp al, 0							; check for zero termination
	jne .next_char

	mov cx, 0x0007						
	mov dx, 0xA120						; 0x0007 A120 is 500,000 microseconds
	call _wait_16r

	call _clear_screen_16r

.init_prompt;
	mov al, '>'
	call _print_char_16r

.prompt:
	call _read_char_16r
	call _print_char_16r
	cmp al, 'X'
	jne .prompt

	call _clear_screen_16r
	jmp .init_prompt

	jmp $								; hang

	v_string db 'STRAWBERRY OS 0.0',0		; Put our string in memory

	%include "inc/subs_16r.asm"

	times 510-($-$$) db 0				; padding
	dw 0xAA55							; standard PC boot sig
