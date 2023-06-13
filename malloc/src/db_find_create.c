#include "main.h"

// Find a defined mmap that has enough room for size bytes
static t_mmap* db_mmap_find_usable(const t_db* db, size_t size)
{
	for (size_t i = 0; i < db->mmaps_i; i++) // TODO: reverse iteration?
	{
		t_mmap* mmap = &db->mmaps[i];
		if (mmap_remaining_size(mmap) >= size)
			return mmap;
	}
	return NULL;
}

// Create a new mmap with size bytes
static t_mmap* db_mmap_create(t_db* db, size_t size)
{
	if (db->mmaps_i >= db_mmap_capacity(db))
		db_mmaps_grow(db);
	t_mmap* mmap = db->mmaps + db->mmaps_i++;
	*mmap = mmap_construct(size);
	return mmap;
}

t_mmap* db_mmap_find_create(t_db* db, size_t size)
{
	t_mmap* mmap = db_mmap_find_usable(db, size);
	if (mmap)
		return mmap;
	return db_mmap_create(db, size);
}

// ==================
// bins
// ==================

// find a bin that can be reused
static t_bin* db_bin_find(const t_db* db, e_status status, size_t size)
{
	assert(status == FREE || status == UNMAPPED);

	for (size_t i = 0; i < db_bin_capacity(db); i++) // TODO: reverse iteration?
	{
		t_bin* bin = &db->bins[i];
		if (bin->status != status)
			continue;

		if (status == FREE && bin->size >= size)
			return bin;
		if (status == UNMAPPED)
			return bin;
	}
	return NULL;
}

static t_bin* db_bin_create(t_db* db, size_t size)
{
	const size_t capacity = db_bin_capacity(db);
	db_bins_grow(db);

	t_bin*	bin = db->bins + capacity;
	t_mmap* mmap = db_mmap_find_create(db, size);
	*bin = bin_construct(db, mmap, size);

	return bin;
}

t_bin* db_bin_find_create(t_db* db, size_t size)
{
	t_bin* bin = db_bin_find(db, FREE, size);
	if (bin)
	{
		bin->status = USED;
		return bin;
	}
	bin = db_bin_find(db, UNMAPPED, size);
	if (bin)
	{
		t_mmap* mmap = db_mmap_find_create(db, size);
		*bin = bin_construct(db, mmap, size);
		bin->status = USED;
		return bin;
	}

	return db_bin_create(db, size);
}

// find a bin by its pointer
// returns NULL if not found
t_bin* db_bin_find_p(t_db* db, void* p)
{
	for (size_t i = 0; i < db_bin_capacity(db); i++)
	{
		t_bin* bin = &db->bins[i];
		if (bin->status != UNMAPPED && bin->p == p)
			return bin;
	}
	return NULL;
}
