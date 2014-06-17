#include "MemoryManagement.h"


void* JSMalloc(size_t size)
{
    void *pointer = malloc(size);
    return pointer;
}

void JSFree(void* pointer)
{
    free(pointer);
}
