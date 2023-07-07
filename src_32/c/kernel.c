/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "include/types.h"
#include "include/macros.h"


#define ROWS            25
#define COLS            80
#define SCREEN_SIZE     ROWS * COLS


extern void __asm_print_char_at(char c, u32 col, u32 row);
extern void __asm_print_char_at_color(char c, u32 col, u32 row, char color);


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

void TEST_va_args(int count, ...) {
    int i;
    va_list args;

    va_start(args, count);  // Initialize the argument list.

    for (i = 0; i < count; i++) {
        int temp = va_arg(args, int);  // Get the next argument value as int.
        char ch = (char) temp;  // Cast int to char.
        print_char_at(ch, i, 0);
    }

    va_end(args);  // Clean up.
}

void TEST_print_char_at_color()
{
    char color = 0x0;

    int x, y;
    for(y = 0; y < ROWS; y++)
    {
        for(x = 0; x < COLS; x++)
        {
            print_char_at_color('*', x, y, color);
            color++;
            if(color == 0xFF)
            {
                color = 0x0;
            }
        }
    }

    print_char_at_color('J', 3, 0, 0x17);
    print_char_at_color('O', 5, 1, 0x17);
    print_char_at('N', 7, 2); //, 0b00010111);
}

void clear_screen()
{
    int i;
    for(i = 0; i < SCREEN_SIZE; i++)
    {
        print_char_at(' ', i, 0);
    }
}


void main()
{
    clear_screen();

    //TEST_va_args(4, 'A', 'B', 'C', 'D');

    //char *str1 = "This is a test!";
    //print_string_at(str1, 5, 2);

    TEST_print_char_at_color();
}