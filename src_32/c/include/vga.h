/*************************************************************************************//*
/
/ JON-STR 7-19-2023
/  - "c the world"
/   Source: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
/
*//*************************************************************************************/


#ifndef VGA_H
#define VGA_H


#include "../types.h"


#define VGA_MEM_ADDR    0xA0000

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		    0x3C1
#define	VGA_INSTAT_READ		0x3DA
#define	VGA_MISC_WRITE		0x3C2
#define	VGA_MISC_READ		0x3CC

/*			                COLOR emulation	 MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		     /* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		     /* 0x3B5 */
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5

#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_SEQ_REGS	5


extern u8 g_320x200x256[];


void vga_test();
void vga_clrscn();
void vga_put_pixel(i32 x, i32 y, u16 color); //i32's maybe be u32s?

void write_regs(unsigned char *regs);


#endif // VGA_H