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

#ifndef size_t
#define size_t uint32_t
#endif


#define COLOR_BLACK  0x0
#define COLOR_GREEN  0x2
#define COLOR_PURPLE 0xF

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

static void vga_xor(u16 color_offset)
{
    int block_size = 4;
    u16 color = 0 + color_offset;
    for(int x = 0; x < 320; x+=block_size)
    {
        color = (++color) % 16;
        for(int y = 0; y < 200; y+=block_size)
        {
            for(int i = 0; i < block_size; i++)
            {
                vga_put_pixel(x + i, y + i, color);
            }
        }
    }
}

u8 back_buffer[320 * 240];

void vga_put_pixel(i32 x, i32 y, u16 color) 
{
    u16 offset = x + 320 * y;
    back_buffer[offset] = color;
}

void vga_flip() 
{
    memcpy((void*)VGA_MEM_ADDR, back_buffer, sizeof(back_buffer));
}

void vga_clrscn() 
{
    for (int i = 0; i < 320; i++) 
    {
        for (int j = 0; j < 200; j++) 
        {
            vga_put_pixel(i, j, COLOR_BLACK);
        }
    }
    vga_flip();
}

void dirty_wait()
{
    for(u64 i = 0; i < 100; i++);
}

void vga_test() 
{
    //println("Attempting to switch modes...", 29);
    write_regs(g_320x200x256);
    vga_clrscn();
    while(true)
    {
        for(int color = 0; color < 16; color++)
        {
            //vga_clrscn();
            vga_xor(color);
            vga_flip();
            dirty_wait();
        }
    }
    /*
	// draw rectangle
	draw_rectangle(150, 10, 100, 50);
	// draw some faces
	draw_happy_face(10,10);
	draw_happy_face(100,100);
	draw_happy_face(300,150);
	// bounds
	vga_put_pixel(0, 0, 15);
	vga_put_pixel(319, 199, COLOR_PURPLE);
	// see some colors
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 100; j++) {
			vga_put_pixel(i, 50+j, i);
		}
	}
    */
}




/*
void vga_put_pixel(i32 x, i32 y, u16 color) {
    u16 offset = x + 320 * y;
    u8 *VGA = (u8*) VGA_MEM_ADDR;
    VGA[offset] = color;
}
*/


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
/* lock 16-color palette and unblank display */
	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}