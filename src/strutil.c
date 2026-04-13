/********************************
 Progres - Verilog Simulator
 (C) 2014-2026 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "lex.h"
#include "strutil.h"

int anexa(char* str, char c)
{
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';

    // TODO: use safer implement.
    strncat(str, tmp, 2);

    // TODO: some checks...

    return 1;
}

int iguais(const char* a, const char* b)
{
	// TODO: use safer implement.
    return !strcmp(a, b);
}

int has_only_digits(const char* str)
{
    unsigned int i;

    if (!str)
        return 0;

    for ( i=0; i < len(str); i++ )
    {
        if ( !isdigit(str[i]) ) {
            return 0;
        }
    }

    return 1;
}

int is_valid_natural_number(const char* str)
{
    // TODO: another function for negative numbers, if needed

    if (!str)
        return 0;

    // is important to check the maximum allowed size for a number literal
    if ( !has_only_digits(str) || !(len(str) <= MAX_DIGITOS_NUM) ) {
        return 0;
    }

    return 1;
}

size_t len(const char* str)
{
    // TODO: restrict to a maximum length
    return strlen(str);
}

char* copy(char* dest, const char* src)
{
    // TODO: restrict to a maximum length
    return strcpy(dest, src);
}

char* copy_removing_quotes(char* dest, const char* src)
{
    char str[MAX_TOKEN_SIZE] = "";

    if ( src[0] != '"' || src[len(src) - 1] != '"' ) {
        // if not between quotes, just copy
        return copy(dest, src);
    }

    copy(str, src + 1); // remove the first quote
    str[len(src) - 2] = '\0'; // remove the last quote

    return copy(dest, str);
}

void print(const char* fmt, ...)
{
    if (global_silent_mode) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void remove_underscores_from_literal(char* str)
{
    // TODO: add test for it

    char* src = str;
    char* dst = str;

    while (*src)
    {
        if (*src != '_') {
            *dst = *src;
            dst++;
        }

        src++;
    }

    *dst = '\0';
}
