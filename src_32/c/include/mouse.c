/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "mouse.h"


#include "isr.h"
#include "string.h"
#include "sys_io.h"
#include "basic_io.h"

#define MOUSE_IRQ 12
#define MOUSE_DATA_PORT 0x60
#define MOUSE_COMMAND_PORT 0x64

// Mouse packet variables
u8 mouse_cycle=0;
char mouse_packet[3];
char mouse_x=0;
char mouse_y=0;

// Initialize the mouse driver
void init_mouse()
{
    outb(PIC1_DATA_PORT, 0xEF);  // enable IRQ12
    register_interrupt_handler(MOUSE_IRQ, &mouse_callback);
    
    // Perform a series of commands to enable the mouse
    outb(MOUSE_COMMAND_PORT, 0xA8); // Enable auxiliary device
    outb(MOUSE_COMMAND_PORT, 0x20); // Command 0x20 sends the command byte to the data port
    u8 status_byte = (inb(MOUSE_DATA_PORT) | 2); // Enable the IRQ12 by setting the bit 1 of the command byte
    outb(MOUSE_COMMAND_PORT, 0x60); // Set the command byte
    outb(MOUSE_DATA_PORT, status_byte);
    
    outb(MOUSE_COMMAND_PORT, 0xD4); // D4 will send the next command to the mouse
    outb(MOUSE_DATA_PORT, 0xF4); // Enable the mouse
}

// Handles the mouse interrupt by reading the data from the port and processing it.
void mouse_callback()
{
    outb(PIC1_COMMAND_PORT, 0x20); // ACK to the PIC

    mouse_packet[mouse_cycle++] = inb(MOUSE_DATA_PORT);

    if (mouse_cycle == 3) { // if we have all the 3 bytes
        mouse_cycle = 0; // set the counter to 0
        mouse_x = mouse_packet[1]; // update mouse_x
        mouse_y = mouse_packet[2]; // update mouse_y
        
         char mouse_x_str[12];
        char mouse_y_str[12];
        itoa(mouse_x, mouse_x_str, 10);
        itoa(mouse_y, mouse_y_str, 10);

        // Print mouse coordinates at the last row of the screen
        print_string_at("Mouse Position: X=", 0, SCREEN_ROWS - 1);
        print_string_at(mouse_x_str, 19, SCREEN_ROWS - 1);
        print_string_at(", Y=", 21 + strlen(mouse_x_str), SCREEN_ROWS - 1);
        print_string_at(mouse_y_str, 24 + strlen(mouse_x_str), SCREEN_ROWS - 1);
    }
}
