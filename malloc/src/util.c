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

// safe mmap
void* ft_mmap(void* addr, size_t length)
{
	assert(length % PAGE_SIZE == 0);

	void* map = mmap(addr, length, PROT_READ | PROT_WRITE, MAP_ANON | MAP_SHARED, -1, 0);
	assert(map != MAP_FAILED);

	return map;
}

// Assert size is valid for mmap
void assert_valid_mmap_size(size_t size)
{
	assert(size > 0);
	assert(size % PAGE_SIZE == 0);
}
