/*************************************************************************************//*
/
/ JON-STR 7-7-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#ifndef TYPES_H
#define TYPES_H

/* TYPES */
/* UNSIGNED */
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

/* SIGNED */
typedef signed char         int8_t;
typedef short               int16_t;
typedef int                 int32_t;
typedef long long           int64_t;

/* BOOLEAN */
#define   bool  unit8_t
#define   true  1
#define   false 0

/* SHORTHAND */
#define   u8    uint8_t
#define   u16   uint16_t
#define   u32   uint32_t
#define   u64   uint64_t

#define   i8    int8_t
#define   i16   int16_t
#define   i32   int32_t
#define   i64   int64_t

#endif