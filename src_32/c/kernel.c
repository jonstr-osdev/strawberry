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
#include "include/mouse.h"
#include "include/idt.h"
#include "include/timer.h"

#include "include/multiboot_info.h"


int main(multiboot_info_t *mb_boot_ptr)
{
    init_descriptor_tables();

    clrscn();

    puts("This is a new Hello, World!");

    //INTERRUPT(0x03);
    //INTERRUPT(0x04);

    enable_interrupts();

    //init_timer(50);

    vga_test();
    
    init_keyboard();

    init_mouse();


    return 0;
}