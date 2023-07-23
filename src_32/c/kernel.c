/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#include "include/descriptor_tables.h"

#include "include/types.h"

#include "include/system.h"
#include "include/vga.h"
#include "include/idt.h"
#include "include/multiboot_info.h"

#include "include/basic_io.h"

#include "include/keyboard.h"
#include "include/mouse.h"
#include "include/timer.h"


int main(multiboot_info_t *mb_boot_ptr)
{
    init_descriptor_tables();

    //clrscn();

    // isr_handler() test
    //INTERRUPT(0x03);
    //INTERRUPT(0x04);

    enable_interrupts();

    init_timer(50);

    vga_test();
    
    // both do not work at the same time right now
    //init_mouse();
    //init_keyboard();

    return 0;
}