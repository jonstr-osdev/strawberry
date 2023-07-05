; **************************************************************************************
;
; JON-STR 7-5-2023
;  - "lol we wrote our own print string..."
;
; **************************************************************************************


[BITS 16]

; hex printing lookup table
_HEX_VALUES db "0123456789ABCDEF"
_HEX_PREFIX db "0x", 0

; disk read error messages
_MSG_DISK_READ_ERROR db "ERROR READING DISK! Status: ", 0
_MSG_SECTOR_READ_ERROR db "ERROR READING SECTOR(s) FROM DISK!"


_print_char_16r:								; (args: al is char to print)
	mov ah, 0x0E
	int 0x10
	ret


_read_char_16r:							
	mov ah, 0x00
	int 0x16
	ret											; (return: al is read char)


_print_string_16r:								; (args: si is base addr of string)
	pusha
.ps16r_next_char:
	lodsb										; Move the character at the address in SI into AL and inc si
	or al, al									; check for zero termination
	jz .ps16r_done
	call _print_char_16r
	jmp .ps16r_next_char

.ps16r_done:
	popa
	ret


_print_hex_prefix_16r:							; (args: bx = 1 then do, bx = 0 then skip)
	or bx, bx
	jz .php16r_done

	mov si, _HEX_PREFIX
	call _print_string_16r

.php16r_done:
	ret


_print_hex_word_16r: 							; (args: ax is word to print, bx is use prefix)
	call _print_hex_prefix_16r

    push ax
    mov al, ah
	xor bx, bx
    call _print_hex_byte_16r

    pop ax
	xor bx, bx
    call _print_hex_byte_16r

    ret


_print_hex_byte_16r:							; (args: al is byte to print, bx is use prefix)
	call _print_hex_prefix_16r

.phb16r_start:
    mov  ah, al
    shr  ah, 4
    push ax
    call _print_hex_nibble_2_16r

    pop  ax 
    mov  ah, al
    and  ah, 0x0F
    call _print_hex_nibble_2_16r

    ret


_print_hex_nibble_16r:							; (args: ah is nibble to print)
    xor  bx, bx
    mov  bl, ah
    mov  al, [_HEX_VALUES + bx]

    call _print_char_16r
    ret


_print_hex_nibble_2_16r:						; (args: ah is nibble to print)
	cmp ah, 0xA
	jl .phn216r_isNumber

	add ah, 'A'
	sub ah, 10
	jmp .phn216r_done

.phn216r_isNumber:
	add ah, '0'

.phn216r_done:
	mov al, ah
	call _print_char_16r
	ret


_set_mode_16r:									; BIOS set vid mode; (args: bl is mode)
 	mov ah, 0x0									; always zero for mode setting
	int 0x10
	ret


_set_text_mode_16r:
	mov al, 0x03								; text mode, 80x25 
	call _set_mode_16r
	ret


_clear_screen_16r:
	call _set_text_mode_16r
	ret


_set_vga_mode_16r:
 	mov al, 0x13
	call _set_mode_16r
	ret


_wait_16r:										; (args: cx:dx is time in microseconds)
	mov ah, 0x86								; wait
	int 0x15									; wait interupt
	ret


_enable_cursor_16r:
	mov ah, 0x01
	mov ch, 0x0
	mov cl, 15
	int 0x10
	ret


_move_cursor_16r:								; (args: dh is column, dl is row)
	mov ah, 0x02
	mov bh, 0x0
	int 0x10
	ret


_disk_read_16r:
	call _clear_screen_16r

	push ax

	mov ah, 0x2
	mov cl, 0x2
	mov ch, 0x0
	mov dl, 0x0
	mov dh, 0x0

	int 0x13
	jc .rd16r_disk_error

	pop bx
	cmp al, bl
	jne .rd16r_sector_error

	ret

.rd16r_disk_error:
	mov si, _MSG_DISK_READ_ERROR
	call _print_string_16r
    mov ah, 0x01          						; get last drive operation status
    int 0x13									; al now contains the status.
	mov bx, 1
	call _print_hex_byte_16r					; printing status
    jmp $

.rd16r_sector_error:
	mov si, _MSG_SECTOR_READ_ERROR
	call _print_string_16r
	jmp $


_reboot_16r:
	int 0x19