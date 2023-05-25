#include "main.h"
#include "util.h"

void create_memory_map(t_memory_maps* maps, size_t minumum_size)
{
	assert(maps->mmaps_len + 10 < MAX_ALLOCATIONS);

	size_t size = nearest_multiple_of(minumum_size, PAGE_SIZE);
	void*  p = ft_mmap(NULL, size);
	t_mmap mmap = {
		.start = p,
		.end = p,
		.capacity = size,
		.uses = 0};
	maps->mmaps[maps->mmaps_len++] = mmap;
}

size_t mmap_bin_remaining(const t_mmap* mmap)
{
	return mmap->capacity - (mmap->end - mmap->start);
}

// @param mmap: the mmap in which the bin is located
t_bin bin_create(t_memory_maps* maps, t_mmap* mmap, size_t size)
{
	assert(mmap_bin_remaining(mmap) >= size);

	t_bin bin = {
		.p = mmap->end,
		.mmap = mmap->start,
		.size = size};
	mmap->end += size;
	mmap->uses++;
	maps->used[maps->used_len++] = bin;
	return bin;
}

t_bin append_bin_to_mmap(t_memory_maps* maps, size_t size)
{
	for (size_t i = 0; i < maps->mmaps_len; i++) // TODO: reverse iteration order?
	{
		t_mmap* mmap = &maps->mmaps[i];
		if (mmap_bin_remaining(mmap) >= size)
			return bin_create(maps, mmap, size);
	}
	create_memory_map(maps, size);
	return append_bin_to_mmap(maps, size);
}

t_bin bin_upsert(t_memory_maps* maps, size_t size)
{
	// TODO: search in free blocks

	return append_bin_to_mmap(maps, size);
}
