/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef BASIC_IO_H
#define BASIC_IO_H

#include "types.h"


#ifndef SCREEN_ROWS
#define SCREEN_ROWS         80
#endif

#ifndef SCREEN_COLS
#define SCREEN_COLS         24
#endif


/* OLD BASIC_IO_H */
void print_char_at(char c, u32 col, u32 row);
void print_char_at_color(char c, u32 col, u32 row, char color);
void print_string_at(char *str, u32 col, u32 row);
/* END OLD BASIC_IO_H*/

void putc(char c);
void puts(char *s);
void clrscn();
void puti(u32 n);

#endif