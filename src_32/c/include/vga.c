/*************************************************************************************//*
/
/ JON-STR 7-19-2023
/  - "c the world"
/   Source: https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
/
*//*************************************************************************************/


#include "vga.h"

#include "sys_io.h"
#include "string.h"
#include "timer.h"

#ifndef size_t
#define size_t uint32_t
#endif


#define COLOR_BLACK  0x0
#define COLOR_GREEN  0x2
#define COLOR_PURPLE 0xF
#define MAX_COLORS   256

static u8 *VID_MEM_BUF = (u8 *) VGA_MEM_ADDR;

//double buffers
u8 _sbuffers[2][VGA_SCREEN_SIZE];
u8 _sback = 0;

#define CURRENT (_sbuffers[_sback])
#define SWAP() (_sback = 1 - _sback)


u8 g_320x200x256[] = {
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

unsigned char g_640x480x16[] = {
/* MISC */
	0xE3,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

static void draw_rectangle(int x, int y, int width, int height) 
{
	for (int i = 0; i < width; i++) 
    {
		for (int j = 0; j < height; j++) 
        {
			vga_put_pixel(x+i, y+j, COLOR_GREEN);
		}
	}
}

static void draw_happy_face(int x, int y) 
{
	// eye
	vga_put_pixel(x, y ,COLOR_PURPLE);
	// eye
	vga_put_pixel(x+10, y,COLOR_PURPLE);
	// mouth
	vga_put_pixel(x,	y+8,COLOR_PURPLE);
	vga_put_pixel(x+1,	y+9,COLOR_PURPLE);
	vga_put_pixel(x+2,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+3,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+4,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+5,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+6,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+7,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+8,	y+10,COLOR_PURPLE);
	vga_put_pixel(x+9,	y+9,COLOR_PURPLE);
	vga_put_pixel(x+10, y+8,COLOR_PURPLE);
}

static void vga_xor()
{
    for(int x = 0; x < VGA_SCREEN_WIDTH; x++)
    {
        for(int y = 0; y < VGA_SCREEN_HEIGHT; y++)
        {
        	u8 color = VALUE_TO_COLOR(x ^ y);
			vga_put_pixel(x, y, color);
        }
    }
}

void vga_put_pixel(i32 x, i32 y, u16 color) 
{
    u16 offset = x + VGA_SCREEN_WIDTH * y;
    CURRENT[offset] = color;
}

void vga_flip() 
{
    memcpy((void*)VID_MEM_BUF, &CURRENT, VGA_SCREEN_SIZE);
	SWAP();
}


void vga_clrscn() 
{
    vga_filscn(COLOR(0, 0, 0));
}

void vga_filscn(u32 color) 
{
    for (int i = 0; i < VGA_SCREEN_WIDTH; i++) 
    {
        for (int j = 0; j < VGA_SCREEN_HEIGHT; j++) 
        {
            vga_put_pixel(i, j, color);
        }
    }
    vga_flip();
}

void dirty_wait()
{
    for(u64 i = 0; i < 10000000; i++);
}

void wait(u32 last_time)
{
	u32 now = get_ticks();
	while(now - last_time > 500)
	{
		;
	}
}

void vga_test() 
{
    //println("Attempting to switch modes...", 29);
    write_regs(g_320x200x256);
    vga_clrscn();
	vga_xor();
	vga_flip();
}

void write_regs(u8 *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	outb(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}

/* Configure 256 color pallette */
	outb(VGA_AC_INDEX, 0x20);

	outb(VGA_PALETTE_MASK, 0xFF);
    outb(VGA_PALETTE_WRITE, 0);
    for (u8 i = 0; i < 255; i++) {
        outb(VGA_PALETTE_DATA, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outb(VGA_PALETTE_DATA, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outb(VGA_PALETTE_DATA, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outb(VGA_PALETTE_DATA, 0x3F);
    outb(VGA_PALETTE_DATA, 0x3F);
    outb(VGA_PALETTE_DATA, 0x3F);
}