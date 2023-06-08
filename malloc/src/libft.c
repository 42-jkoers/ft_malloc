#include "main.h"

void* ft_memcpy(void* dest, const void* src, size_t n)
{
	unsigned char* d;
	unsigned char* s;

	if (dest == NULL || src == NULL || dest == src)
		return (dest);
	d = dest;
	s = (void*)src;
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return (dest);
}
