/*************************************************************************************//*
/
/ JON-STR 7-23-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "timer.h"

#include "isr.h"

#include "basic_io.h"
#include "sys_io.h"


#define PIT_FREQ 1193180

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40


u32 tick = 0;


static void timer_callback(registers_t regs)
{
    tick++;
    puts("Tick: ");
    puti(tick);
    puts("\n");
}


void init_timer(u32 freq)
{
    register_interrupt_handler(IRQ0, &timer_callback);

    u32 div = PIT_FREQ / freq;


    outb(PIT_COMMAND_PORT, 0x36);

    u8 lowb  = (u8) (div & 0xFF);
    u8 highb = (u8)((div >> 8) & 0xFF);


    outb(PIT_CHANNEL0_PORT, lowb);
    outb(PIT_CHANNEL0_PORT, highb);
}
