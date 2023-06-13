#include "main.h"
t_mmap mmap_construct(size_t min_size)
{
	t_mmap mmap;
	size_t size = nearest_multiple_of(min_size, PAGE_SIZE);
	void*  p = ft_mmap(NULL, size);
	mmap.start = p;
	mmap.end = p;
	mmap.capacity = size;
	mmap.uses = 0;
	return mmap;
}

size_t mmap_remaining_size(const t_mmap* mmap)
{
	if (!mmap->start)
		return 0;
	return mmap->capacity - (mmap->end - mmap->start);
}
