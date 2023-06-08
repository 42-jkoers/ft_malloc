#include "main.h"
size_t mmap_remaining_size(const t_mmap* mmap)
{
	if (!mmap->start)
		return 0;
	return mmap->capacity - (mmap->end - mmap->start);
}
