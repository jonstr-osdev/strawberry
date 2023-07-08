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

#include "include/keyboard_map.h"


#define     IDT_SIZE                        256
#define     IDT_INTERRUPT_GATE_32           0x8E

#define     KERNEL_CODE_SEGMENT_OFFSET      0x8 //0x8

#define     PIC1_COMMAND_PORT               0x20
#define     PIC1_DATA_PORT                  0x21
#define     PIC2_COMMAND_PORT               0xA0
#define     PIC2_DATA_PORT                  0xA1

#define     KEYBOARD_DATA_PORT              0x60
#define     KEYBOARD_STATUS_PORT            0x64


extern void __asm_load_IDT(u32 idt_addr);

struct IDT_ptr {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct IDT_entry {
    u16 offset_lower_bits;
    u16 selector;
    u8  zero;   //always zero ? (i beleeb so)
    u8 type_attr;
    u16 offset_upper_bits;
} __attribute__((packed));

struct IDT_entry IDT[IDT_SIZE];
int cursor_pos = 0;

extern void __asm_enable_interrupts();

extern void __asm_keyboard_handler();

extern i8   __asm_io_port_in(u16 port);
extern void __asm_io_port_out(u16 port, u8 data);



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

void init_IDT()
{
    u32 offset = (u32) __asm_keyboard_handler;
    IDT[0x21].offset_lower_bits = offset & 0x0000FFFF;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = IDT_INTERRUPT_GATE_32;
    IDT[0x21].offset_upper_bits = (offset & 0xFFFF0000) >> 16;

    //ICW1
    __asm_io_port_out(PIC1_COMMAND_PORT, 0x11);
    __asm_io_port_out(PIC2_COMMAND_PORT, 0x11);

    //ICW2
    __asm_io_port_out(PIC1_DATA_PORT, 0x20);
    __asm_io_port_out(PIC2_DATA_PORT, 0x28);

    //ICW3
    __asm_io_port_out(PIC1_DATA_PORT, 0x0);
    __asm_io_port_out(PIC2_DATA_PORT, 0x0);

    //ICW4
    __asm_io_port_out(PIC1_DATA_PORT, 0x1);
    __asm_io_port_out(PIC2_DATA_PORT, 0x1);

    //mask interrupts
    __asm_io_port_out(PIC1_DATA_PORT, 0xFF);
    __asm_io_port_out(PIC2_DATA_PORT, 0xFF);

    struct IDT_ptr idt_ptr;
	idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
	idt_ptr.base = (unsigned int) &IDT;
	// Now load this IDT
	__asm_load_IDT(&idt_ptr);

}

void init_keyboard()
{
    __asm_io_port_out(PIC1_DATA_PORT, 0xFD);
}

void handle_keyboard_interrupt()
{
    __asm_io_port_out(PIC1_COMMAND_PORT, 0x20);
    u8 status = __asm_io_port_in(KEYBOARD_STATUS_PORT);

    if(status & 0x1)
    {
        i8 keycode = __asm_io_port_in(KEYBOARD_DATA_PORT);
        if(keycode < 0 || keycode >= 128)
        {
            return;
        }

        print_char_at(keyboard_map[keycode], cursor_pos, 0);
        cursor_pos++;
    }
}

void enable_interrupts()
{
    __asm_enable_interrupts();
}

void main()
{
    clear_screen();

    //TEST_va_args(4, 'A', 'B', 'C', 'D');

    //char *str1 = "This is a test!";
    //print_string_at(str1, 5, 2);

    TEST_print_char_at_color();

    init_IDT();
    init_keyboard();
    enable_interrupts();
    while(true)
    {

    }
}