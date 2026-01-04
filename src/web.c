#ifdef __EMSCRIPTEN__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <emscripten.h>
//#include <emscripten/bind.h> // C++11

#include "estruturas.h"
#include "verilog.h"

EMSCRIPTEN_KEEPALIVE
Module* w_load_module(const char* source_code, size_t length)
{
    FILE * stream;

    stream = fmemopen((void*)source_code, length, "r");

    if (stream == NULL) {
        perror("fmemopen failed");
        return NULL;
    }

    return load_module(stream, NULL, NULL);
}

#ifdef __cplusplus
}
#endif

#endif
