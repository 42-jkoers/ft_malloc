#include "main.h"

t_memory_maps* memory_maps_singleton()
{
	static t_memory_maps maps = {.bins = NULL};
	if (!maps.bins)
		maps = memory_maps_construct();
	return &maps;
}

t_memory_maps memory_maps_construct()
{
	t_memory_maps maps;

	maps.bins_size = nearest_multiple_of(10000 * sizeof(t_bin), PAGE_SIZE);
	maps.bins = ft_mmap(NULL, maps.bins_size);
	maps.bins_len = 0;

	maps.mmaps_size = nearest_multiple_of(10000 * sizeof(t_mmap), PAGE_SIZE);
	maps.mmaps = ft_mmap(NULL, maps.mmaps_size);
	maps.mmaps_len = 0;
	return maps;
}

// how may t_mmap structs can fit in the memory_maps.mmaps_size
size_t memory_maps_mmap_capacity(const t_memory_maps* maps)
{
	return maps->mmaps_size / sizeof(t_mmap);
}

// how may t_bin structs can fit in the memory_maps.bins_size
size_t memory_maps_bin_capacity(const t_memory_maps* maps)
{
	return maps->bins_size / sizeof(t_bin);
}

void memory_maps_destruct(t_memory_maps* maps)
{
	ft_munmap(maps->bins, maps->bins_size);
	ft_munmap(maps->mmaps, maps->mmaps_size);
}
