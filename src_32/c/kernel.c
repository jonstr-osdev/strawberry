/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "include/descriptor_tables.h"

#include "include/types.h"
#include "include/macros.h"

#include "include/basic_io.h"
#include "include/sys_io.h"
#include "include/vga.h"

#include "include/keyboard.h"
#include "include/idt.h"
#include "include/timer.h"

//#include "include/idt.h"


void TEST_print_char_at_color()
{
    char color = 0x0;

    int x, y;
    for(y = 0; y < SCREEN_ROWS; y++)
    {
        for(x = 0; x < SCREEN_COLS; x++)
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


int main(struct muliboot *mb_boot_ptr)
{
    init_descriptor_tables();

    clrscn();

    puts("This is a new Hello, World!");

    //INTERRUPT(0x03);
    //INTERRUPT(0x04);

    enable_interrupts();

    //init_timer(50);

    init_keyboard();

    //init_mouse();

    vga_test();

    return 0;
}