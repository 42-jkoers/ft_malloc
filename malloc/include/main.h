#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#define INITIAL_MAX_ALLOCATION 1000
#define BIN_TINY 16
#define PAGE_SIZE 4096

typedef enum
{
	FAIL,
	SUCCESS,
} e_result;

typedef enum
{
	FREE,
	USED,
	UNMAPPED,
} e_status;

typedef struct
{
	void*  start;
	void*  end;
	size_t capacity; // in bytes
	size_t uses;
} t_mmap;
t_mmap mmap_construct(size_t min_size);
size_t mmap_remaining_size(const t_mmap* mmap);

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
	size_t	bins_size; // in bytes

	t_mmap* mmaps;
	size_t	mmaps_size; // in bytes
	size_t	mmaps_i;
} t_db;

t_db	db_construct();
void	db_destruct(t_db* db);
t_db*	db_singleton();
size_t	db_mmap_capacity(const t_db* db);
size_t	db_bin_capacity(const t_db* maps);
void	db_mmaps_grow(t_db* db);
void	db_bins_grow(t_db* db);
t_bin*	db_bin_find_create(t_db* db, size_t size);
void	db_bin_release(t_db* db, t_bin* bin);
t_bin*	db_bin_find_p(t_db* db, void* p);
t_bin	bin_construct(const t_db* db, t_mmap* mmap, size_t size); // TODO: forward declare
t_mmap* bin_get_mmap(const t_db* db, const t_bin* bin);

void*	ft_mmap(void* addr, size_t length);
void*	ft_mmap_grow(void* map, size_t curr_size, size_t new_size);

void*	ft_memcpy(void* dest, const void* src, size_t n);
size_t	nearest_multiple_of(size_t number, size_t multiple);
void	ft_munmap(void* addr, size_t length);
