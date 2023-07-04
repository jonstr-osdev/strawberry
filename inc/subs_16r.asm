[BITS 16]

_set_mode_16r:							; BIOS set vid mode; (args: bl is mode)
 	mov ah, 0x0							; always zero for mode setting
	mov al, bl							; text mode = 0x03, vga mode = 0x13
	int 0x10
	ret


_set_text_mode_16r:
	mov bl, 0x03						; text mode, 80x25 
	call _set_mode_16r
	ret


_clear_screen_16r:
	call _set_text_mode_16r
	ret


_set_vga_mode_16r:
 	mov bl, 0x13
	call _set_mode_16r
	ret


_wait_16r:
	mov ah, 0x86						; wait
	mov cx, 0x0007						; cx:dx is time
	mov dx, 0xA120						; 0x0007 A120 is 500,000 microseconds
	int 0x15							; wait interupt
	ret


_enable_cursor_16r:
	mov ah, 0x01
	mov ch, 0x0
	mov cl, 15
	int 0x10
	ret


_move_cursor_16r:						; (args: ch is column, cl is row)
	mov ah, 0x02
	mov bh, 0x0
	mov dh, ch
	mov dl, cl
	int 0x10
	ret


_reboot_16r:
	int 0x19