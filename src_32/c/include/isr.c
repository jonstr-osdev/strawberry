/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "isr.h"


#include "sys_io.h"
#include "basic_io.h"


#define PIC1_PORT_A 0x21
#define PIC2_PORT_A 0xA1


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
        port = PIC1_PORT_A;
    } 
    else 
    {
        port = PIC2_PORT_A;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);        
}