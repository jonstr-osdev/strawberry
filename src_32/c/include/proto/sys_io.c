/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "sys_io.h"


extern  u8   __asm_io_port_in_b(u16 port);
extern u16   __asm_io_port_in_w(u16 port);

extern void  __asm_io_port_out_b(u16 port, u8 data);


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

