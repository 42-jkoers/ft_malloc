#include "main.h"
#include "util.h"
#include <stdio.h>
#include <time.h>
#define BILLION 1000000000L
size_t allocations = 0;

void   write_test_data(void* ptr, size_t size)
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
	allocations++;
	write_test_data(ptr, size);
	assert_test_data(ptr, size);
	ft_free(ptr);
}

void run_tests()
{
	test_malloc(1);
	test_malloc(100);
	test_malloc(10000);
}

struct timespec start_clock()
{
	struct timespec start_time;
	clock_gettime(CLOCK_MONOTONIC, &start_time);
	return start_time;
}

long long get_duration_ns(struct timespec start_time)
{
	struct timespec end_time;
	clock_gettime(CLOCK_MONOTONIC, &end_time);

	long long ns = BILLION * (end_time.tv_sec - start_time.tv_sec) + end_time.tv_nsec - start_time.tv_nsec;
	return ns;
}

int main()
{
	struct timespec time = start_clock();
	for (size_t i = 0; i < 100; i++)
	{
		run_tests();
	}
	long long duration = get_duration_ns(time);
	print_allocations();
	printf("Test passed\n");
	printf("  Allocations       : %lu\n", allocations);
	const float allocs_per_sec = (float)allocations / (float)duration * BILLION;
	printf("  Allocations/second: %.1e - %f\n", allocs_per_sec, allocs_per_sec);
	printf("  Duration          : %lld ns\n", duration);
	printf("  Duration          : %lld ms\n", duration / 1000000);

	return 0;
}
