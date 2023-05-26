#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void* ft_malloc(size_t);
extern void	 ft_free(void*);
extern void	 print_allocations();
