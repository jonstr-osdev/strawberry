/*************************************************************************************//*
/
/ JON-STR 7-12-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef IDT_H
#define IDT_H

#define     IDT_SIZE                        256
#define     IDT_INTERRUPT_GATE_32           0x8E

#include "types.h"

void init_IDT();
void enable_interrupts();


#endif //IDT_H