/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "descriptor_tables.h"

#include "jm_gdt.h"
#include "jm_idt.h"


void init_descriptor_tables()
{
    init_gdt();
    init_idt();
}