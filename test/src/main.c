#include "main.h"
#include "util.h"
#include <stdio.h>

void write_test_data(void* ptr, size_t size)
{
	size_t	i = 0;
	uint8_t c = 0;

	while (i < size)
	{
		((uint8_t*)ptr)[i] = c++;
		i++;
	}
}

void assert_test_data(void* ptr, size_t size)
{
	size_t	i = 0;
	uint8_t c = 0;

	while (i < size)
	{
		if (((uint8_t*)ptr)[i] != c)
		{
			printf("Test data does not match at index %lu\n", i);
			printf("Expected: %d\n", c);
			printf("Got     : %d\n", ((uint8_t*)ptr)[i]);
			exit(1);
		}
		c++;
		i++;
	}
}

void test_malloc(size_t size)
{
	void* ptr = ft_malloc(size);
	write_test_data(ptr, size);
	assert_test_data(ptr, size);
	// ft_free(ptr);
}

void run_tests()
{
	test_malloc(1);
	test_malloc(100);
	test_malloc(10000);
}

int main()
{
	for (size_t i = 0; i < 100; i++)
	{
		run_tests();
	}

	printf("Test passed\n");
	return 0;
}
