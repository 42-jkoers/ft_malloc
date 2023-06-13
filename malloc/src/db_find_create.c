#include "main.h"

// if bin is free and big enough to store size amount of bytes
static bool bin_should_be_reused(const t_bin* bin, size_t size)
{
	if (bin->status != FREE)
		return false;
	if (bin->size < size) // TODO: this is wasteful if bin->size >>> size
		return false;
	return true;
}

// Get place to store mmap data
t_mmap* db_find_create_mmap(t_db* db)
{
	for (size_t i = 0; i < db->mmaps_len; i++) // TODO: reverse iteration?
	{
		t_mmap* mmap = &db->mmaps[i];
		if (!mmap->start) // if mmap is unused
			return mmap;
	}
	if (db->mmaps_len >= db_mmap_capacity(db))
		db_mmaps_grow(db);

	return &db->mmaps[db->mmaps_len++];
}

t_bin db_create_bin(t_db* db, size_t size)
{
	for (size_t i = 0; i < db->mmaps_len; i++) // TODO: reverse iteration?
	{
		t_mmap* mmap = &db->mmaps[i];
		if (mmap_remaining_size(mmap) >= size)
		{
			t_bin bin = bin_construct(db, mmap, size);
			if (db->bins_len >= db_bin_capacity(db))
				db_bins_grow(db);
			db->bins[db->bins_len++] = bin;
			return bin;
		}
	}
	// No room in no mmap, create new mmap
	db_add_memory_map(db, size);
	return db_create_bin(db, size);
}

// find a bin by its pointer
// returns NULL if not found
t_bin* db_find_bin(t_db* db, void* p)
{
	for (size_t i = 0; i < db->bins_len; i++)
	{
		t_bin* bin = &db->bins[i];
		if (bin->p == p)
			return bin;
	}
	return NULL;
}

// find a reusable bin that fits a new allocation of size bytes
// returns NULL if not found
t_bin* db_find_reusable_bin(t_db* db, size_t size)
{
	for (size_t i = 0; i < db->bins_len; i++)
	{
		t_bin* bin = &db->bins[i];
		if (bin_should_be_reused(bin, size))
		{
			return bin;
		}
	}
	return NULL;
}

void db_add_memory_map(t_db* db, size_t minumum_size)
{
	const size_t size = nearest_multiple_of(minumum_size, PAGE_SIZE);
	void*		 p = ft_mmap(NULL, size);
	t_mmap*		 mmap = db_find_create_mmap(db);

	mmap->start = p;
	mmap->end = p;
	mmap->capacity = size;
	mmap->uses = 0;
}
