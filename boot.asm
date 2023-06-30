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

	mov ah, 0x0e						; print char bios call
	mov si, v_string					

.next_char:								; Jump here when there's another character
	lodsb								; Move the character at the address in SI into AL and inc si
	int 0x10							; print char

	cmp al, 0							; check for zero termination
	jne .next_char						

	jmp $								; hang

	v_string db 'Hello World!',0		; Put our string in memory

	times 510-($-$$) db 0				; padding
	dw 0xAA55							; standard PC boot sig
