/*************************************************************************************//*
/
/ JON-STR 7-12-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef KEYBOARD_H
#define KEYBOARD_H


#include "types.h"


#define     KEYBOARD_DATA_PORT              0x60
#define     KEYBOARD_STATUS_PORT            0x64


extern u32 __asm_keyboard_handler();

void init_keyboard();
void keyboard_callback();


#endif