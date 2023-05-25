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

t_memory_maps g_allocations = {
	.mmaps_len = 0,
	.bins_len = 0};

void* ft_malloc(size_t size)
{
	t_bin bin = upsert_bin(&g_allocations, size);
	return bin.p;
}

void ft_free(void* ptr)
{
	if (!ptr)
		return;
	t_bin* bin = find_bin(&g_allocations, ptr);
	if (!bin)
		errx(1, "free(): could not find allocation");
	if (bin->status == FREE)
		errx(1, "free(): double free");
	release_bin(&g_allocations, bin);
}
