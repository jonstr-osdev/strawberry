/*************************************************************************************//*
/
/ JON-STR 7-12-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef SYS_IO_H
#define SYS_IO_H

#include "types.h"


#define     KERNEL_CODE_SEGMENT_OFFSET      0x8 //0x8

#define     PIC1_COMMAND_PORT               0x20
#define     PIC1_DATA_PORT                  0x21
#define     PIC2_COMMAND_PORT               0xA0
#define     PIC2_DATA_PORT                  0xA1


void outb(u16 port, u8 value);

u8  inb(u16 port);
u16 inw(u16 port);

#endif //SYS_IO_H