#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <common.h>
#include <efi/efilib.h>

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif // __MALLOC_H__