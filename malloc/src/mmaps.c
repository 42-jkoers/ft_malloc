#include "main.h"

t_db* db_singleton()
{
	static t_db maps = {.bins = NULL};
	if (!maps.bins)
		maps = db_construct();
	return &maps;
}

t_db db_construct()
{
	t_db maps;

	maps.bins_size = nearest_multiple_of(INITIAL_MAX_ALLOCATION * sizeof(t_bin), PAGE_SIZE);
	maps.bins = ft_mmap(NULL, maps.bins_size);
	maps.bins_len = 0;

	maps.mmaps_size = nearest_multiple_of(INITIAL_MAX_ALLOCATION * sizeof(t_mmap), PAGE_SIZE);
	maps.mmaps = ft_mmap(NULL, maps.mmaps_size);
	maps.mmaps_len = 0;

	return maps;
}

// how may t_mmap structs can fit in the mmaps.mmaps_size
size_t db_mmap_capacity(const t_db* maps)
{
	return maps->mmaps_size / sizeof(t_mmap);
}

// how may t_bin structs can fit in the mmaps.bins_size
size_t db_bin_capacity(const t_db* maps)
{
	return maps->bins_size / sizeof(t_bin);
}

void db_mmaps_grow(t_db* maps)
{
	const size_t new_size = maps->mmaps_size * 2;
	maps->mmaps = ft_mmap_grow(maps->mmaps, maps->mmaps_size, new_size);
	maps->mmaps_size = new_size;
}

void db_bins_grow(t_db* maps)
{
	printf("growing bins\n");
	const size_t new_size = maps->bins_size * 2;
	maps->bins = ft_mmap_grow(maps->bins, maps->bins_size, new_size);
	maps->bins_size = new_size;
}

void db_destruct(t_db* maps)
{
	ft_munmap(maps->bins, maps->bins_size);
	ft_munmap(maps->mmaps, maps->mmaps_size);
}
