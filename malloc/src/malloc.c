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

// TODO: remove
t_debug g_debug = {.mmap_calls = 0};
//

void* ft_malloc(size_t size)
{
	if (size < BIN_TINY)
		size = BIN_TINY;
	t_bin* bin = db_bin_find_create(db_singleton(), size);
	return bin->p;
}

void ft_free(void* ptr)
{
	if (!ptr)
		return;
	t_bin* bin = db_bin_find_p(db_singleton(), ptr);
	if (!bin)
		errx(1, "free(): could not find allocation");
	if (bin->status == FREE)
		errx(1, "free(): double free");
	db_bin_release(db_singleton(), bin);
}

void print_allocations()
{
	printf("number of mmaps in use: %lu\n", db_singleton()->mmaps_i);

	size_t bins_count = 0;
	for (size_t i = 0; i < db_bin_capacity(db_singleton()); i++)
	{
		t_bin* bin = &db_singleton()->bins[i];
		if (bin->status == USED)
			bins_count++;
	}
	printf("number of bins in use : %lu\n", bins_count);
	printf("number of mmap calls  : %lu\n", g_debug.mmap_calls);
}
