/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#ifndef STRING_H
#define STRING_H


#include <stddef.h>

#include "types.h"


#define streq(s1, s2) (strcmp((s1), (s2)) == 0)


void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dst, const void *src, size_t n);

int strcmp(const char *s1, const char *s2);
char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
size_t strlen(const char *s);
char *strncpy(char *dst, const char *src, size_t n);
char *strncat(char *dst, const char *src, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *s, const char *reject);
char *strpbrk(const char *s, const char *accept);

void reverse(char *str, int length);
char *itoa(int num, char *str, int base);

int isspace(char c);
int isdigit(char c);
int ishexdigit_lower(char c);
int ishexdigit_UPPER(char c);

#endif //STRING_H