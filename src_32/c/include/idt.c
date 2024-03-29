/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "idt.h"


#include "isr.h"
#include "string.h"
#include "system.h"


extern void __asm_flush_idt(u32 gdt_ptr);
extern void __asm_enable_interrupts();


static void idt_flush(u32 idt_ptr)
{
    __asm_flush_idt(idt_ptr);
}


idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


static void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}


void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (u32)&idt_entries;

    memset(idt_entries, 0, sizeof(idt_entry_t)*256);

    // Remap the irq table.
    outb(PIC1_COMMAND_PORT, 0x11);    // ICW1: initialize command (start the initialization sequence in cascade mode)
    outb(PIC2_COMMAND_PORT, 0x11);    // ICW1: initialize command (start the initialization sequence in cascade mode)
    outb(PIC1_DATA_PORT,    0x20);    // ICW2: Base address of interrupt vector (IRQ0-IRQ7 -> INT 20h-27h)
    outb(PIC2_DATA_PORT,    0x28);    // ICW2: Base address of interrupt vector (IRQ8-IRQ15 -> INT 28h-2Fh)
    outb(PIC1_DATA_PORT,    0x04);    // ICW3: inform PIC1 that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC2_DATA_PORT,    0x02);    // ICW3: inform PIC2 it is slave on IRQ2 of master PIC1 (0000 0010)
    outb(PIC1_DATA_PORT,    0x01);    // ICW4: set x86 mode
    outb(PIC2_DATA_PORT,    0x01);    // ICW4: set x86 mode
    outb(PIC1_DATA_PORT,    0x0);     // OCW1: clear mask - enable all IRQs on PIC1
    outb(PIC2_DATA_PORT,    0x0);     // OCW1: clear mask - enable all IRQs on PIC2

    idt_set_gate( 0, (u32)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (u32)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (u32)isr2 , 0x08, 0x8E);
    idt_set_gate( 3, (u32)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (u32)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (u32)isr5 , 0x08, 0x8E);
    idt_set_gate( 6, (u32)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (u32)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (u32)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (u32)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32)isr31, 0x08, 0x8E);
    idt_set_gate(32, (u32)irq0, 0x08, 0x8E);
    idt_set_gate(33, (u32)irq1, 0x08, 0x8E);
    idt_set_gate(34, (u32)irq2, 0x08, 0x8E);
    idt_set_gate(35, (u32)irq3, 0x08, 0x8E);
    idt_set_gate(36, (u32)irq4, 0x08, 0x8E);
    idt_set_gate(37, (u32)irq5, 0x08, 0x8E);
    idt_set_gate(38, (u32)irq6, 0x08, 0x8E);
    idt_set_gate(39, (u32)irq7, 0x08, 0x8E);
    idt_set_gate(40, (u32)irq8, 0x08, 0x8E);
    idt_set_gate(41, (u32)irq9, 0x08, 0x8E);
    idt_set_gate(42, (u32)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32)irq15, 0x08, 0x8E);

    idt_flush((u32)&idt_ptr);
}


void enable_interrupts()
{
   __asm_enable_interrupts();
}