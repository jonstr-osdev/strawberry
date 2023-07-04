[BITS 16]

_clear_screen_16r:
	mov ah, 0x00						; BIOS set vid mode
	mov al, 0x03						; vid mode 80x25
	int 0x10							; BIOS set vid mode
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

_move_cursor_16r:						; ch is column, cl is row; manage stack yourself
	mov ah, 0x02
	mov bh, 0x0
	mov dh, ch
	mov dl, cl
	int 0x10
	ret

_reboot_16r:
	int 0x19