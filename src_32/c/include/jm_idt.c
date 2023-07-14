/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "jm_idt.h"


extern void __asm_flush_idt(u32 gdt_ptr);

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
   ///IMPLEMENT
}