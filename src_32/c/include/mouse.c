/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  - source: Free PS/2 Mouse Code: https://forum.osdev.org/viewtopic.php?t=10247
/
*//*************************************************************************************/


#include "mouse.h"


#include "isr.h"
#include "string.h"
#include "sys_io.h"
#include "basic_io.h"
#include "vga.h"
#include "timer.h"


#define MOUSE_IRQ 12
#define MOUSE_DATA_PORT 0x60
#define MOUSE_COMMAND_PORT 0x64


// Mouse packet variables
u8 mouse_cycle=0;
char mouse_packet[3];
char mouse_x=0;
char mouse_y=0;


void mouse_wait(u8 type)
{
    u32 time_out = 100000;

    if(type == 0)
    {
        while(time_out--)
        {
            if((inb(MOUSE_COMMAND_PORT) & 1) == 1)
            {
                return;
            }
        }

        return;
    }
    else
    {
        while(time_out--)
        {
            if((inb(MOUSE_COMMAND_PORT) & 2) == 0)
            {
                return;
            }

            return;
        }
    }
}

void mouse_write(u8 write) //unsigned char
{
    //Wait to be able to send a command
    mouse_wait(1);
    //Tell the mouse we are sending a command
    outb(MOUSE_COMMAND_PORT, 0xD4);
    //Wait for the final part
    mouse_wait(1);
    //Finally write
    outb(MOUSE_DATA_PORT, write);
}

u8 mouse_read()
{
    //Get's response from mouse
    mouse_wait(0);
    return inb(MOUSE_DATA_PORT);
}

// Initialize the mouse driver
void init_mouse()
{
    u8 status;

    // enable aux device
    mouse_wait(1);
    outb(MOUSE_COMMAND_PORT, 0xA8);
    
    unmask_irq(IRQ12);

    // enable interrupt
    mouse_wait(1);
    outb(MOUSE_COMMAND_PORT, 0x20);
    mouse_wait(0);
    status = (inb(MOUSE_DATA_PORT) | 2);
    mouse_wait(1);
    outb(MOUSE_COMMAND_PORT, MOUSE_DATA_PORT);
    mouse_wait(1);
    outb(MOUSE_DATA_PORT, status);

    // use defaults
    mouse_write(0xF6);
    mouse_read();

    //enable device
    mouse_write(0xF4);
    mouse_read();

    register_interrupt_handler(IRQ12, mouse_callback);
}

// Handles the mouse interrupt by reading the data from the port and processing it.
void mouse_callback()
{
    static int absolute_x = 0;
    static int absolute_y = 0;

    static u32 secs = 0;

    switch(mouse_cycle)
    {
        case 0:
            mouse_packet[0]=inb(MOUSE_DATA_PORT);
            mouse_cycle++;
            break;
        case 1:
            mouse_packet[1]=inb(MOUSE_DATA_PORT);
            mouse_cycle++;
            break;
        case 2:
            mouse_packet[2]=inb(MOUSE_DATA_PORT);
            
            // The x and y offsets are signed, need to cast to signed char
            char x_offset = (char)mouse_packet[1];
            char y_offset = (char)mouse_packet[2];
            
            // Update the absolute position of the cursor
            absolute_x += x_offset;
            absolute_y -= y_offset;  // Subtract y offset because positive y is up in screen coordinates
            
            // Clamp absolute x and y to screen boundaries
            // Replace VGA_WIDTH and VGA_HEIGHT with your actual screen size
            if (absolute_x < 0) absolute_x = 0;
            if (absolute_x >= VGA_SCREEN_WIDTH) absolute_x = VGA_SCREEN_WIDTH - 1;
            if (absolute_y < 0) absolute_y = 0;
            if (absolute_y >= VGA_SCREEN_HEIGHT) absolute_y = VGA_SCREEN_HEIGHT - 1;
            
            mouse_cycle=0;

            clrscn();
            puts("Mouse Moved:  X: ");
            puti(absolute_x);
            puts(" , Y: ");
            puti(absolute_y);
            putc('\n');
            if(get_ticks() % 50 == 0)
            {
                puts("Seconds: ");
                puti(secs++);
                putc('\n');
            }
            
        break;
    }
}