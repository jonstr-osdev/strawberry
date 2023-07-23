/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "system.h"
#include "font.h"

#include "basic_io.h"

#include "vga.h"


#define USE_RAND_FAST 1


extern  u8   __asm_io_port_in_b(u16 port);
extern u16   __asm_io_port_in_w(u16 port);

extern void  __asm_io_port_out_b(u16 port, u8 data);


static u32 _rseed = 1;

u8 inb(u16 port)
{
    return __asm_io_port_in_b(port);
}


u16 inw(u16 port)
{
    return __asm_io_port_in_w(port);
}


void outb(u16 port, u8 value)
{
    __asm_io_port_out_b(port, value);
}


void seed(u32 s) 
{
    _rseed = s;
}


static u32 rand_fast() 
{
    static u32 x = 123456789;
    static u32 y = 362436069;
    static u32 z = 521288629;
    static u32 w = 88675123;

    x *= 23786259 - _rseed++;

    u32 t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}


static u32 rand_slow()
{
    static u32 x = 123456789;
    static u32 y = 362436469;
    static u32 z = 521288329;
    static u32 w = 84675123;

    x *= 23786159 - _rseed++;

    u32 t;

    t = x ^ (x << 11);
    x = y; y = z; z = w;
    w = w ^ (w >> 19) ^ t ^ (t >> 8);

    return w * 1812433253;  // a large prime number
}



u32 rand()
{
    if(USE_RAND_FAST)
    {
       return rand_fast();
    }
    else
    {
        return rand_slow();
    }
}


void panic(const char *err) 
{
    /// FIX THIS ///
    vga_filscn(COLOR(7, 0, 0));

    if (err != NULL) {
        font_str(err, VGA_SCREEN_WIDTH - font_width(err) / 2, VGA_SCREEN_HEIGHT / 2 - 4, COLOR(7, 7, 3));
    }

    vga_flip();
    for (;;) {}
}