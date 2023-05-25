#include "main.h"
#include "util.h"
#include <stdio.h>

void write_test_data(void* ptr, size_t size)
{
	size_t	i = 0;
	uint8_t c = 0;

	while (i < size)
	{
		((char*)ptr)[i] = c++;
		i++;
	}
}

bool is_valid_test_data(void* ptr, size_t size)
{
	size_t	i = 0;
	uint8_t c = 0;

	while (i < size)
	{
		if (((char*)ptr)[i] != c++)
			return false;
		i++;
	}
	return true;
}

void assert_test_data(void* ptr, size_t size)
{
	if (!is_valid_test_data(ptr, size))
		EXIT_WITH_ERROR("Test data is not valid");
}

void run_tests()
{
	void* ptr = ft_malloc(100);
	write_test_data(ptr, 100);
	assert_test_data(ptr, 100);
	// ft_free(ptr);
}

int main()
{
	for (size_t i = 0; i < 100; i++)
	{
		run_tests();
	}

	printf("Test passed\n");
}
