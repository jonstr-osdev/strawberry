/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "basic_io.h"


void print_char_at(char c, u32 col, u32 row)
{
    __asm_print_char_at(c, col, row);
}

void print_char_at_color(char c, u32 col, u32 row, char color)
{
    __asm_print_char_at_color(c, col, row, color);
}

void print_string_at(char *str, u32 col, u32 row)
{
    while(*str)
    {
        print_char_at(*str, col, row);
        str++;
        col++;
    }
}