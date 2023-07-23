/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "isr.h"


#include "system.h"
#include "basic_io.h"
#include "idt.h"


isr_t interrupt_handlers[256];


void register_interrupt_handler(u8 n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    puts("recieved interrupt: ");
    puti(regs.int_no);
    putc('\n');

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

void unmask_irq(u8 irq) 
{
    u16 port;
    u8 value;

    if(irq < 8) 
    {
        port = PIC1_DATA_PORT;
    } 
    else 
    {
        port = PIC2_DATA_PORT;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);        
}