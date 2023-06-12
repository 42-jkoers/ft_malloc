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

void* ft_malloc(size_t size)
{
	if (size < BIN_TINY)
		size = BIN_TINY;
	t_bin bin = upsert_bin(db_singleton(), size);
	return bin.p;
}

void ft_free(void* ptr)
{
	if (!ptr)
		return;
	t_bin* bin = find_bin(db_singleton(), ptr);
	if (!bin)
		errx(1, "free(): could not find allocation");
	if (bin->status == FREE)
		errx(1, "free(): double free");
	release_bin(db_singleton(), bin);
}

void print_allocations()
{
	size_t mmaps_count = 0;
	for (size_t i = 0; i < db_singleton()->mmaps_len; i++)
	{
		t_mmap* mmap = &db_singleton()->mmaps[i];
		if (mmap->start)
		{
			printf("mmap in use %p %lu\n", mmap->start, i);
			mmaps_count++;
		}
	}
	printf("number of mmaps in use: %lu\n\n", mmaps_count);

	size_t bins_count = 0;
	for (size_t i = 0; i < db_singleton()->bins_len; i++)
	{
		t_bin* bin = &db_singleton()->bins[i];
		if (bin->status == USED)
		{
			printf("bin in use  %p %lu\n", bin->p, i);
			bins_count++;
		}
	}
	printf("number of bins in use : %lu\n\n", bins_count);
}
