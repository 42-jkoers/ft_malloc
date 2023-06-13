#include "main.h"

// public functions

t_db* db_singleton()
{
	static t_db db = {.bins = NULL};
	if (!db.bins)
		db = db_construct();
	return &db;
}

t_db db_construct()
{
	t_db db;

	db.bins_size = nearest_multiple_of(INITIAL_MAX_ALLOCATION * sizeof(t_bin), PAGE_SIZE);
	db.bins = ft_mmap(NULL, db.bins_size);
	db.bins_len = 0;

	db.mmaps_size = nearest_multiple_of(INITIAL_MAX_ALLOCATION * sizeof(t_mmap), PAGE_SIZE);
	db.mmaps = ft_mmap(NULL, db.mmaps_size);
	db.mmaps_len = 0;

	return db;
}

// how may t_mmap structs can fit in the mmaps.mmaps_size
size_t db_mmap_capacity(const t_db* db)
{
	return db->mmaps_size / sizeof(t_mmap);
}

// how may t_bin structs can fit in the mmaps.bins_size
size_t db_bin_capacity(const t_db* db)
{
	return db->bins_size / sizeof(t_bin);
}

void db_mmaps_grow(t_db* db)
{
	const size_t new_size = db->mmaps_size * 2;
	db->mmaps = ft_mmap_grow(db->mmaps, db->mmaps_size, new_size);
	db->mmaps_size = new_size;
}

void db_bins_grow(t_db* db)
{
	const size_t new_size = db->bins_size * 2;
	db->bins = ft_mmap_grow(db->bins, db->bins_size, new_size);
	db->bins_size = new_size;
}

void db_destruct(t_db* db)
{
	ft_munmap(db->bins, db->bins_size);
	ft_munmap(db->mmaps, db->mmaps_size);
}

// marking all bins in this mmap as unmapped memory
void db_unmap_bins(t_db* db, const t_mmap* mmap)
{
	// TODO: can this be optimized?
	for (size_t i = 0; i < db->bins_len; i++) // TODO: reverse iteration?
	{
		t_bin* bin = &db->bins[i];
		if (bin_get_mmap(db, bin) == mmap)
			bin->status = UNMAPPED;
	}
}

void db_unmap_mmap(t_db* db, t_mmap* mmap)
{
	assert(mmap >= db->mmaps && mmap < db->mmaps + db->mmaps_len);

	db_unmap_bins(db, mmap);
	ft_munmap(mmap->start, mmap->capacity);
	mmap->start = NULL;
}

void release_bin(t_db* db, t_bin* bin)
{
	bin->status = FREE;

	t_mmap* mmap = bin_get_mmap(db, bin);
	if (--mmap->uses == 0)
		db_unmap_mmap(db, mmap);
}

t_bin db_upsert_bin(t_db* db, size_t size)
{
	// first try to find a already allocated bin that can be reused
	t_bin* bin = db_find_reusable_bin(db, size);
	if (bin)
	{
		bin->status = USED;
		return *bin;
	}
	// if no reusable bin found, create a new one
	return db_create_bin(db, size);
}
