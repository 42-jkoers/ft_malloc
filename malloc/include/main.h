#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_ALLOCATIONS 1000
#define BIN_SMALL 32
#define PAGE_SIZE 4096

typedef struct
{
	void*  start;
	void*  end;
	size_t capacity; // in bytes
	size_t uses;
} t_mmap;

typedef enum
{
	FREE,
	USED,
	UNMAPPED,
} e_status;

typedef struct
{
	void*	 p;
	t_mmap*	 mmap;
	size_t	 size;
	e_status status;
} t_bin;

typedef struct
{
	t_bin  bins[MAX_ALLOCATIONS];
	size_t bins_len;
	t_mmap mmaps[MAX_ALLOCATIONS];
	size_t mmaps_len;
} t_memory_maps;

void*  ft_mmap(void* addr, size_t length);
size_t nearest_multiple_of(size_t number, size_t multiple);
void   assert_valid_mmap_size(size_t size);
t_bin  upsert_bin(t_memory_maps* maps, size_t size);
void   release_bin(t_memory_maps* maps, t_bin* bin);
void   ft_munmap(void* addr, size_t length);
t_bin* find_reusable_bin(t_memory_maps* maps, size_t size);
t_bin* find_bin(t_memory_maps* maps, void* p);
