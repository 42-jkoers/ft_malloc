#include "main.h"

t_mmaps* mmaps_singleton()
{
	static t_mmaps maps = {.bins = NULL};
	if (!maps.bins)
		maps = mmaps_construct();
	return &maps;
}

t_mmaps mmaps_construct()
{
	t_mmaps maps;

	maps.bins_size = nearest_multiple_of(10000 * sizeof(t_bin), PAGE_SIZE);
	maps.bins = ft_mmap(NULL, maps.bins_size);
	maps.bins_len = 0;

	maps.mmaps_size = nearest_multiple_of(10000 * sizeof(t_mmap), PAGE_SIZE);
	maps.mmaps = ft_mmap(NULL, maps.mmaps_size);
	maps.mmaps_len = 0;
	return maps;
}

// how may t_mmap structs can fit in the mmaps.mmaps_size
size_t mmaps_mmap_capacity(const t_mmaps* maps)
{
	return maps->mmaps_size / sizeof(t_mmap);
}

// how may t_bin structs can fit in the mmaps.bins_size
size_t mmaps_bin_capacity(const t_mmaps* maps)
{
	return maps->bins_size / sizeof(t_bin);
}

void mmaps_grow_mmap(t_mmaps* maps)
{
	const size_t new_size = maps->mmaps_size * 2;
	maps->mmaps = ft_mmap_grow(maps->mmaps, maps->mmaps_size, new_size);
	maps->mmaps_size = new_size;
}

void mmaps_grow_bins(t_mmaps* maps)
{
	const size_t new_size = maps->bins_size * 2;
	maps->bins = ft_mmap_grow(maps->bins, maps->bins_size, new_size);
	maps->bins_size = new_size;
}

void mmaps_destruct(t_mmaps* maps)
{
	ft_munmap(maps->bins, maps->bins_size);
	ft_munmap(maps->mmaps, maps->mmaps_size);
}
