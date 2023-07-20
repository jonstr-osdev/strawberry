/*************************************************************************************//*
/
/ JON-STR 7-11-2023
/  - "c the world"
/  
/
*//*************************************************************************************/

#ifdef _MCTYPE_H
#define _MCTYPE_H


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


#endif