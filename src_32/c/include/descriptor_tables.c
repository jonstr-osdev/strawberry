/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "descriptor_tables.h"

// Lets us access our ASM functions from our C code.
extern void __asm_flush_gdt(u32 gdt_ptr);
extern void __asm_flush_idt(u32 gdt_ptr);

static void gdt_flush(u32 gdt_ptr)
{
    __asm_flush_gdt(gdt_ptr);
}

static void idt_flush(u32 idt_ptr)
{
    __asm_flush_idt(idt_ptr);
}

// Internal function prototypes.
static void init_gdt();
static void gdt_set_gate(i32, u32, u32, u8, u8);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;



static void init_idt();
static void idt_set_gate(u8,u32,u16,u8);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void init_descriptor_tables()
{
  init_gdt();
  init_idt();
}

static void init_gdt()
{
   gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
   gdt_ptr.base  = (u32)&gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush((u32)&gdt_ptr);
}

// Set the value of one GDT entry.
static void gdt_set_gate(i32 num, u32 base, u32 limit, u8 access, u8 gran)
{
   gdt_entries[num].base_low    = (base & 0xFFFF);
   gdt_entries[num].base_middle = (base >> 16) & 0xFF;
   gdt_entries[num].base_high   = (base >> 24) & 0xFF;

   gdt_entries[num].limit_low   = (limit & 0xFFFF);
   gdt_entries[num].granularity = (limit >> 16) & 0x0F;

   gdt_entries[num].granularity |= gran & 0xF0;
   gdt_entries[num].access      = access;
}

static void init_idt()
{
   idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
   idt_ptr.base  = (u32)&idt_entries;

//   memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

//    idt_set_gate( 0, (u32)isr0 , 0x08, 0x8E);
//    idt_set_gate( 1, (u32)isr1 , 0x08, 0x8E);

//    idt_set_gate(31, (u32)isr32, 0x08, 0x8E);

   idt_flush((u32)&idt_ptr);
}

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