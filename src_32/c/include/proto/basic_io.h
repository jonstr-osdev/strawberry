/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef BASIC_IO_H
#define BASIC_IO_H


#include "../types.h"


#define SCREEN_ROWS     25
#define SCREEN_COLS     80
#define SCREEN_SIZE     SCREEN_ROWS * SCREEN_COLS


void print_char_at(char c, u32 col, u32 row);
void print_char_at_color(char c, u32 col, u32 row, char color);
void print_string_at(char *str, u32 col, u32 row);

void clear_screen();

#endif