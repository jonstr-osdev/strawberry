/*************************************************************************************//*
/
/ JON-STR 7-12-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "idt.h"

#include "proto/sys_io.h"

#include "keyboard.h"


extern void __asm_load_IDT(u32 idt_addr);
static void load_IDT(u32 idt_addr)
{
    __asm_load_IDT(idt_addr);
}

extern void __asm_enable_interrupts();


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

void init_IDT()
{
    u32 offset = (u32) __asm_keyboard_handler;
    IDT[0x21].offset_lower_bits = offset & 0x0000FFFF;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = IDT_INTERRUPT_GATE_32;
    IDT[0x21].offset_upper_bits = (offset & 0xFFFF0000) >> 16;

    //ICW1
    outb(PIC1_COMMAND_PORT, 0x11);
    outb(PIC2_COMMAND_PORT, 0x11);

    //ICW2
    outb(PIC1_DATA_PORT, 0x20);
    outb(PIC2_DATA_PORT, 0x28);

    //ICW3
    outb(PIC1_DATA_PORT, 0x0);
    outb(PIC2_DATA_PORT, 0x0);

    //ICW4
    outb(PIC1_DATA_PORT, 0x1);
    outb(PIC2_DATA_PORT, 0x1);

    //mask interrupts
    outb(PIC1_DATA_PORT, 0xFF);
    outb(PIC2_DATA_PORT, 0xFF);

    struct IDT_ptr idt_ptr;
	idt_ptr.limit = (sizeof(struct IDT_entry) * IDT_SIZE) - 1;
	idt_ptr.base = (unsigned int) &IDT;
	// Now load this IDT
	load_IDT((u32) &idt_ptr);
}

void enable_interrupts()
{
    __asm_enable_interrupts();
}