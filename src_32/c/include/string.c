/*************************************************************************************//*
/
/ JON-STR 7-13-2023
/  - "c the world"
/  
/
*//*************************************************************************************/


#include "string.h"

#include "types.h"


#include <stdbool.h>


void *memcpy(void *dst, const void *src, size_t n) 
{
    
    char *d = dst;
    const char *s = src;
    
    while (n--) 
    {
        *d++ = *s++;
    }
    return dst;
}

void *memset(void *s, int c, size_t n) 
{
    u8 *p = s;
    u8 value = (u8)c;
    while(n--) 
    {
        *p++ = value;
    }
    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) 
{
    const u8 *p1 = s1;
    const u8 *p2 = s2;
    while (n--) 
    {
        if (*p1 != *p2)
        {
            return *p1 - *p2;
        }
        ++p1;
        ++p2;
    }
    return 0;
}

void *memmove(void *dst, const void *src, size_t n) 
{
    u8 *d = dst;
    const u8 *s = src;
    if (d < s)
    {
        while (n--)
        {
            *d++ = *s++;
        }
    }
    else 
    {
        d += n;
        s += n;
        while (n--)
        {
            *--d = *--s;
        }
    }
    return dst;
}

int strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        ++s1;
        ++s2;
    }
    return *(u8 *)s1 - *(u8 *)s2;
}


char *strcpy(char *dst, const char *src) 
{
    char *d = dst;
    while (*d++ = *src++) ;  /* copy src over dst */
    return dst;  /* return dst */
}


char *strcat(char *dst, const char *src) 
{
    char *d = dst;
    while (*d) 
    {
        ++d;    /* find end of dst */
    }
    while (*d++ = *src++) ;  /* copy src to end of dst */
    return dst;  /* return dst */
}


size_t strlen(const char *s) 
{
    const char *p = s;
    while (*p) 
    {
        ++p;
    }
    return p - s;
}

char *strncpy(char *dst, const char *src, size_t n) 
{
    char *d = dst;
    while (n && (*d++ = *src++)) /* copy src over dst */
    {
        --n;
    }
    while (n) /* fill the rest with '\0' */
    {
        *d++ = '\0';
        --n;
    }
    return dst;
}

char *strncat(char *dst, const char *src, size_t n) 
{
    char *d = dst;
    while (*d) 
    {
        ++d; /* find end of dst */
    }
    while (n && (*d++ = *src++)) /* copy src to end of dst */
    {
        --n;
    }
    if (n == 0)
    {
        *d = '\0'; /* null-terminate dst */
    }
    return dst;
}

int strncmp(const char *s1, const char *s2, size_t n) 
{
    while (n && *s1 && (*s1 == *s2)) 
    {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return *(u8 *)s1 - *(u8 *)s2;
    }
}

char *strchr(const char *s, int c) 
{
    while (*s != (char)c) 
    {
        if (!*s++)
        {
            return 0; /* return NULL if c was not found */
        }
    }
    return (char *)s;
}

char *strrchr(const char *s, int c) 
{
    char *res = NULL;
    do 
    {
        if (*s == (char)c)
        {
            res = (char *)s;
        }
    } while (*s++);
    return res;
}

char *strstr(const char *haystack, const char *needle) 
{
    if (!*needle)
    {
        return (char *)haystack;
    }
    for (; *haystack; ++haystack) 
    {
        if (*haystack == *needle && !strncmp(haystack, needle, strlen(needle))) 
        {
            return (char *)haystack;
        }
    }
    return NULL;
}

size_t strspn(const char *s, const char *accept) 
{
    size_t len = 0;
    for (; *s; ++s) 
    {
        if (!strchr(accept, *s))
        {
            break;
        }
        ++len;
    }
    return len;
}

size_t strcspn(const char *s, const char *reject) 
{
    size_t len = 0;
    for (; *s; ++s) 
    {
        if (strchr(reject, *s))
        {
            break;
        }
        ++len;
    }
    return len;
}

char *strpbrk(const char *s, const char *accept) 
{
    for (; *s; ++s) 
    {
        if (strchr(accept, *s))
        {
            return (char *)s;
        }
    }
    return NULL;
}

void reverse(char* str, int length) {
    int start = 0;
    int end = length -1;
    char temp;
    while (start < end) {
        temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

int isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int isdigit(char c)
{
    return (c >= '0' && c <= '9');
}

int ishexdigit_lower(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
}

int ishexdigit_UPPER(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
}
