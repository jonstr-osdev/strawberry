[BITS 16]

_print_char_16r:						; (args: al is char to print)
	mov ah, 0x0E
	int 0x10
	ret

_read_char_16r:							
	mov ah, 0x00
	int 0x16
	ret									; (return: al is read char)

_set_mode_16r:							; BIOS set vid mode; (args: bl is mode)
 	mov ah, 0x0							; always zero for mode setting
	int 0x10
	ret


_set_text_mode_16r:
	mov al, 0x03						; text mode, 80x25 
	call _set_mode_16r
	ret


_clear_screen_16r:
	call _set_text_mode_16r
	ret


_set_vga_mode_16r:
 	mov al, 0x13
	call _set_mode_16r
	ret


_wait_16r:								; (args: cx:dx is time)
	mov ah, 0x86						; wait
	int 0x15							; wait interupt
	ret


_enable_cursor_16r:
	mov ah, 0x01
	mov ch, 0x0
	mov cl, 15
	int 0x10
	ret


_move_cursor_16r:						; (args: dh is column, dl is row)
	mov ah, 0x02
	mov bh, 0x0
	int 0x10
	ret


_reboot_16r:
	int 0x19