#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define MAX_ALLOCATIONS 1000
#define BIN_TINY 16
#define PAGE_SIZE 4096

typedef enum
{
	FAIL,
	SUCCESS,
} e_result;

typedef struct
{
	void*  start;
	void*  end;
	size_t capacity; // in bytes
	size_t uses;
} t_mmap;
size_t mmap_remaining_size(const t_mmap* mmap);

typedef enum
{
	FREE,
	USED,
	UNMAPPED,
} e_status;

typedef struct
{
	void*	 p;
	size_t	 mmap_offset;
	size_t	 size;
	e_status status;
} t_bin;

typedef struct
{
	t_bin*	bins;
	size_t	bins_len;
	size_t	bins_size; // in bytes
	t_mmap* mmaps;
	size_t	mmaps_len;
	size_t	mmaps_size;
} t_mmaps;
t_mmaps	 mmaps_construct();
void	 mmaps_destruct(t_mmaps* maps);
t_mmaps* mmaps_singleton();
size_t	 mmaps_mmap_capacity(const t_mmaps* maps);
size_t	 mmaps_bin_capacity(const t_mmaps* maps);
void	 mmaps_grow_mmaps(t_mmaps* maps);
void	 mmaps_grow_bins(t_mmaps* maps);

// todo forward declare t_mmaps
t_bin	bin_construct(const t_mmaps* maps, t_mmap* mmap, size_t size);
t_mmap* bin_get_mmap(t_mmaps* maps, const t_bin* bin);

void*	ft_mmap(void* addr, size_t length);
void*	ft_mmap_grow(void* map, size_t curr_size, size_t new_size);

void*	ft_memcpy(void* dest, const void* src, size_t n);
size_t	nearest_multiple_of(size_t number, size_t multiple);
void	assert_valid_mmap_size(size_t size);
t_bin	upsert_bin(t_mmaps* maps, size_t size);
void	release_bin(t_mmaps* maps, t_bin* bin);
void	ft_munmap(void* addr, size_t length);
t_bin*	find_reusable_bin(t_mmaps* maps, size_t size);
t_bin*	find_bin(t_mmaps* maps, void* p);
