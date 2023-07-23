/*************************************************************************************//*
/
/ JON-STR 7-12-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "keyboard.h"
#include "keyboard_map.h"

#include "basic_io.h"

#include "system.h"
#include "isr.h"
#include "idt.h"


static int cursor_pos = 0;

void init_keyboard()
{
    outb(PIC1_DATA_PORT, 0xFD);

    register_interrupt_handler(IRQ1, &keyboard_callback);
}

void keyboard_callback()
{
    outb(PIC1_COMMAND_PORT, 0x20);
    u8 status = inb(KEYBOARD_STATUS_PORT);

    if(status & 0x1)
    {
        i8 keycode = inb(KEYBOARD_DATA_PORT);
        if(keycode < 0 || keycode >= 128)
        {
            return;
        }

        /* changed this code to ferry chars it gets to some sort of
           shell or wherever control should be
           ideally in the future, the keyboard doesnt only print to the screen and can do more*/
        print_char_at(keyboard_map[keycode], cursor_pos, 0);
        cursor_pos++;
    }
}