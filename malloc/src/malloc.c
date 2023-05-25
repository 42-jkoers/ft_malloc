#include "main.h"
#include "util.h"
#include <err.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

t_memory_maps g_allocations = {.mmaps_len = 0, .used_len = 0};

void*		  ft_malloc(size_t size)
{
	t_bin bin = bin_upsert(&g_allocations, size);
	return bin.p;
}
