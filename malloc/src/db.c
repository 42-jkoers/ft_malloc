#include "main.h"
#include "util.h"

// Get place to store mmap data
t_mmap* find_create_mmap(t_mmaps* maps)
{
	for (size_t i = 0; i < maps->mmaps_len; i++) // TODO: reverse iteration?
	{
		t_mmap* mmap = &maps->mmaps[i];
		if (!mmap->start) // if mmap is unused
		{
			return mmap;
		}
	}
	// No reusable mmap found, create new one
	return &maps->mmaps[maps->mmaps_len++];
}

void add_memory_map(t_mmaps* maps, size_t minumum_size)
{
	assert(maps->mmaps_len + 10 < MAX_ALLOCATIONS);

	size_t	size = nearest_multiple_of(minumum_size, PAGE_SIZE);
	void*	p = ft_mmap(NULL, size);

	t_mmap* mmap = find_create_mmap(maps);
	mmap->start = p;
	mmap->end = p;
	mmap->capacity = size;
	mmap->uses = 0;
}

t_bin create_bin(t_mmaps* maps, size_t size)
{
	for (size_t i = 0; i < maps->mmaps_len; i++) // TODO: reverse iteration?
	{
		t_mmap* mmap = &maps->mmaps[i];
		if (mmap_remaining_size(mmap) >= size)
		{
			t_bin bin = bin_construct(maps, mmap, size);
			maps->bins[maps->bins_len++] = bin;
			return bin;
		}
	}
	// No room in no mmap, create new mmap
	add_memory_map(maps, size);
	return create_bin(maps, size);
}

// marking all bins in this mmap as unmapped memory
void unmap_bins(t_mmaps* maps, const t_mmap* mmap)
{
	// TODO: can this be optimized?
	for (size_t i = 0; i < maps->bins_len; i++) // TODO: reverse iteration?
	{
		t_bin* bin = &maps->bins[i];
		if (bin_get_mmap(maps, bin) == mmap)
			bin->status = UNMAPPED;
	}
}

void unmap_mmap(t_mmaps* maps, t_mmap* mmap)
{
	assert(mmap >= maps->mmaps && mmap < maps->mmaps + maps->mmaps_len);

	unmap_bins(maps, mmap);
	ft_munmap(mmap->start, mmap->capacity);
	mmap->start = NULL;
}

void release_bin(t_mmaps* maps, t_bin* bin)
{
	bin->status = FREE;

	t_mmap* mmap = bin_get_mmap(maps, bin);
	if (--mmap->uses == 0)
		unmap_mmap(maps, mmap);
}

// if bin is free and big enough to store size amount of bytes
bool bin_should_be_reused(const t_bin* bin, size_t size)
{
	if (bin->status != FREE)
		return false;
	if (bin->size < size) // TODO: this is wasteful if bin->size >>> size
		return false;
	return true;
}

// find a reusable bin by its pointer
// returns NULL if not found
t_bin* find_bin(t_mmaps* maps, void* p)
{
	for (size_t i = 0; i < maps->bins_len; i++)
	{
		t_bin* bin = &maps->bins[i];
		if (bin->p == p)
			return bin;
	}
	return NULL;
}

// find a reusable bin that fits a new allocation of size bytes
// returns NULL if not found
t_bin* find_reusable_bin(t_mmaps* maps, size_t size)
{
	for (size_t i = 0; i < maps->bins_len; i++)
	{
		t_bin* bin = &maps->bins[i];
		if (bin_should_be_reused(bin, size))
		{
			return bin;
		}
	}
	return NULL;
}

t_bin upsert_bin(t_mmaps* maps, size_t size)
{
	// first try to find a already allocated bin that can be reused
	t_bin* bin = find_reusable_bin(maps, size);
	if (bin)
	{
		bin->status = USED;
		return *bin;
	}
	// if no reusable bin found, create a new one
	return create_bin(maps, size);
}
