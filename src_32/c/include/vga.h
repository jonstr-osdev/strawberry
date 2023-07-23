/*************************************************************************************//*
/
/ JON-STR 7-19-2023
/  - "c the world"
/   Source: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
/
*//*************************************************************************************/


#ifndef VGA_H
#define VGA_H


#include "types.h"


#define VGA_MEM_ADDR        0xA0000

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

#define VGA_SCREEN_WIDTH    320
#define VGA_SCREEN_HEIGHT   200
#define VGA_SCREEN_SIZE     320 * 200


#define COLOR(_r, _g, _b)((u8)( \
    (((_r) & 0x7) << 5) |       \
    (((_g) & 0x7) << 2) |       \
    (((_b) & 0x3) << 0)))

#define COLOR_R(_index) (((_index) >> 5) & 0x7)
#define COLOR_G(_index) (((_index) >> 2) & 0x7)
#define COLOR_B(_index) (((_index) >> 0) & 0x3)

#define COLOR_ADD(_index, _d) __extension__({   \
        __typeof__(_index) _c = (_index);       \
        __typeof__(_d) __d = (_d);              \
        COLOR(                                  \
            CLAMP(COLOR_R(_c) + __d, 0, 7),     \
            CLAMP(COLOR_G(_c) + __d, 0, 7),     \
            CLAMP(COLOR_B(_c) + __d, 0, 3)      \
        );})

extern u8 _sbuffers[2][VGA_SCREEN_SIZE];
extern u8 _sback;

#define screen_buffer() (_sbuffers[_sback])

#define screen_set(_p, _x, _y)\
    (_sbuffers[_sback][((_y) * VGA_SCREEN_WIDTH + (_x))]=(_p))

#define screen_offset(_x, _y) (screen_buffer()[(_y) * VGA_SCREEN_WIDTH + (_x)])

#define screen_fill(_c, _x, _y, _w, _h) do {\
        __typeof__(_x) __x = (_x);\
        __typeof__(_y) __y = (_y);\
        __typeof__(_w) __w = (_w);\
        __typeof__(_y) __ymax = __y + (_h);\
        __typeof__(_c) __c = (_c);\
        for (; __y < __ymax; __y++) {\
            memset(&screen_buffer()[__y * VGA_SCREEN_WIDTH + __x], __c, __w);\
        }\
    } while (0)


extern u8 g_320x200x256[];


void vga_test();
void vga_filscn(u32 color);
void vga_clrscn();
void vga_put_pixel(i32 x, i32 y, u16 color); //i32's maybe be u32s?
void vga_flip();

void write_regs(unsigned char *regs);


#endif // VGA_H