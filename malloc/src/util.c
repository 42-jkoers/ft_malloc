#include "util.h"
#include "main.h"
#include <sys/mman.h>

size_t nearest_multiple_of(size_t number, size_t multiple)
{
	size_t remainder = number % multiple;
	if (remainder == 0)
		return number;
	return number + multiple - remainder;
}

static void* ft_mmap_unsafe(void* addr, size_t length)
{
	assert(length % PAGE_SIZE == 0);
	return mmap(addr, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
}

// safe mmap
void* ft_mmap(void* addr, size_t length)
{
	void* map = ft_mmap_unsafe(addr, length);
	assert(map != MAP_FAILED);
	assert(map);
	return map;
}

e_result ft_mmap_try_grow(void* addr, size_t length)
{
	assert(length % PAGE_SIZE == 0);

	void* map = ft_mmap_unsafe(addr, length);
	if (map == MAP_FAILED)
		return FAIL;
	if (!map)
		return FAIL;
	return SUCCESS;
}

// attemps to grow the mmap, if it fails
// it will create a new map and copy the old map into it
// and free the old map
void* ft_mmap_grow(void* map, size_t curr_size, size_t new_size)
{
	assert(curr_size % PAGE_SIZE == 0);
	assert(new_size % PAGE_SIZE == 0);

	if (ft_mmap_try_grow(map, new_size) == SUCCESS)
		return map;

	void* new_map = ft_mmap(NULL, new_size);
	ft_memcpy(new_map, map, curr_size);
	ft_munmap(map, curr_size);
	return new_map;
}

void ft_munmap(void* addr, size_t length)
{
	assert(length % PAGE_SIZE == 0);
	assert(munmap(addr, length) == 0);
}

// Assert size is valid for mmap
void assert_valid_mmap_size(size_t size)
{
	assert(size > 0);
	assert(size % PAGE_SIZE == 0);
}
