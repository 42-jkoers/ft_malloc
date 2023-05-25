#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_ALLOCATIONS 1000
#define BIN_SMALL 32
#define PAGE_SIZE 4096

void* ft_malloc(size_t size); // TODO: rename to malloc()

typedef struct
{
	void*  p;
	void*  mmap;
	size_t size;
} t_bin;

typedef struct
{
	void*  start;
	void*  end;
	size_t capacity; // in bytes
	size_t uses;
} t_mmap;

typedef struct
{
	t_bin  used[MAX_ALLOCATIONS * (4096 / BIN_SMALL)];
	size_t used_len;
	t_mmap mmaps[MAX_ALLOCATIONS];
	size_t mmaps_len;
} t_memory_maps;

void*  ft_mmap(void* addr, size_t length);
size_t nearest_multiple_of(size_t number, size_t multiple);
void   assert_valid_mmap_size(size_t size);
t_bin  bin_upsert(t_memory_maps* maps, size_t size);
