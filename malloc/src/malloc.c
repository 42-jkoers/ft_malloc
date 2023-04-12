#include <stdio.h>
#include <stddef.h>

void* ft_malloc(size_t size) {
	printf("%zu", size);
	return NULL;
}
