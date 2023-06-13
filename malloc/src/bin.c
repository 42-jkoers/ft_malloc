#include "main.h"

// @param mmap: the mmap in which the bin is located
t_bin bin_construct(const t_db* maps, t_mmap* mmap, size_t size)
{
	assert(mmap_remaining_size(mmap) >= size);

	t_bin bin = {
		.p = mmap->end,
		.mmap_offset = (size_t)(mmap - maps->mmaps),
		.size = size,
		.status = USED};
	mmap->end += size;
	mmap->uses++; // todo?

	return bin;
}

t_mmap* bin_get_mmap(const t_db* maps, const t_bin* bin)
{
	if (bin->status == UNMAPPED)
		return NULL;
	return &maps->mmaps[bin->mmap_offset];
}
