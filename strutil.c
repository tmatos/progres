/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

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
    strcat(str, tmp);

    // TODO: some checks...

    return 1;
}

int iguais(const char* a, const char* b)
{
	// TODO: use safer implement.
    return !strcmp(a, b);
}

int apenasDigitos(const char* str)
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

int isNumNaturalValido(const char* str)
{
    if (!str)
        return 0;

    // importante nao ser um valor muito grande, esses numeros
    if ( !apenasDigitos(str) || !(len(str) <= MAX_DIGITOS_NUM) ) {
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
