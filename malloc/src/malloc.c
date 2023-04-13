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

#define MAX_ALLOCATIONS 1000
typedef struct
{
	size_t maps;
	void*  mmaps[MAX_ALLOCATIONS];
} t_memory_maps;

t_memory_maps g_allocations = {0, {0}};
//

void add_memory_map(void* map)
{
	if (g_allocations.maps >= MAX_ALLOCATIONS)
		errx(1, "too many allocations");
	g_allocations.mmaps[g_allocations.maps++] = map;
}

void* ft_mmap(void* addr, size_t length)
{
	ft_assert(length % 4096, ==, 0);

	void* map = mmap(addr, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
	if (map == MAP_FAILED)
		errx(1, "mmap");
	return map;
}

size_t nearest_multiple_of(size_t number, size_t multiple)
{
	size_t remainder = number % multiple;
	if (remainder == 0)
		return number;
	return number + multiple - remainder;
}

void* ft_malloc(size_t size)
{
	size = nearest_multiple_of(size, 4096);
	char* p = ft_mmap(NULL, size);
	add_memory_map(p);
	return (void*)p;
}

void ft_free(void* ptr)
{
	for (size_t i = 0; i < g_allocations.maps; i++)
	{
		if (g_allocations.mmaps[i] == ptr)
		{
			munmap(ptr, 4096);
			g_allocations.mmaps[i] = NULL;
			return;
		}
	}
	errx(1, "Attempting to free unallocated memory");
}
