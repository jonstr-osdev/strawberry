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


#define TXT_SCREEN_ROWS         80
#define TXT_SCREEN_COLS         24


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