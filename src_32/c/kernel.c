/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "include/types.h"
#include "include/macros.h"

#include "include/proto/basic_io.h"
#include "include/proto/sys_io.h"
#include "include/proto/jm_io.h"

#include "include/keyboard.h"

#include "include/idt.h"


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


void main(struct muliboot *mb_boot_ptr)
{
    //clear_screen();

    //print_string_at("STRAWBERRY OS 0.0 - JON-STR 7-12-2023", 0, 0);
    //print_char_at('>', 0, 1);

    //fill_screen('*');
    clrscn();
    puts("This is a new Hello, World!");


    init_IDT();
    init_keyboard();
    enable_interrupts();
    while(true)
    {

    }
}