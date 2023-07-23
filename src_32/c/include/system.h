/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef SYSTEM_H
#define SYSTEM_H


#include "types.h"

#define     KERNEL_CODE_SEGMENT_OFFSET      0x8 // y tho?

#define _assert_0() __error_illegal_macro__
#define _assert_1(_e) do { if (!(_e)) panic(NULL); } while (0)
#define _assert_2(_e, _m) do { if (!(_e)) panic((_m)); } while (0)

#define _assert(x, _e, _m, _f, ...) _f

#define assert(...) _assert(,##__VA_ARGS__,\
        _assert_2(__VA_ARGS__),\
        _assert_1(__VA_ARGS__),\
        _assert_0(__VA_ARGS__))


void outb(u16 port, u8 value);

u8  inb(u16 port);
u16 inw(u16 port);

void panic(const char *err);
u32 rand();
void seed(u32 s);


#endif