#include "main.h"
#include "util.h"
#include <stdio.h>
#include <time.h>
#define BILLION 1000000000L
#define MAX_ALLOCATIONS 10000000
size_t	  allocations_count = 0;
size_t	  allocations_i = 0;
void*	  allocations[MAX_ALLOCATIONS];
long long allocated_bytes = 0;

void	  write_test_data(void* ptr, size_t size)
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

void allocate_and_read_write(size_t size)
{
	void* ptr = ft_malloc(size);
	write_test_data(ptr, size);
	assert_test_data(ptr, size);
	allocations[allocations_i++] = ptr;
	allocations_count++;
	allocated_bytes += size;
}

void run_tests()
{
	const size_t iterations = 20;
	allocations_i = 0;
	for (size_t i = 0; i < iterations; i++)
	{
		allocate_and_read_write(1);
		allocate_and_read_write(100);
		allocate_and_read_write(1000);
	}
	for (size_t i = 0; i < allocations_i; i++)
	{
		ft_free(allocations[i]);
	}
}

struct timespec start_clock()
{
	struct timespec start;
	clock_gettime(CLOCK_MONOTONIC, &start);
	return start;
}

long long get_duration_ns(struct timespec start)
{
	struct timespec end;
	clock_gettime(CLOCK_MONOTONIC, &end);

	long long ns = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	return ns;
}

int main()
{
	printf("========================================\n");

	struct timespec time = start_clock();
	for (size_t i = 0; i < 100; i++)
	{
		run_tests();
	}
	long long duration = get_duration_ns(time);
	print_allocations();
	printf("Test passed\n");
	printf("  Allocations       : %.1e - %lu \n", (float)allocations_count, allocations_count);
	const float allocs_per_sec = (float)allocations_count / (float)duration * BILLION;
	printf("  Allocations/second: %.1e - %f\n", allocs_per_sec, allocs_per_sec);
	printf("  Allocated         : %.2f MB - %lld bytes\n", allocated_bytes / 1000000.0, allocated_bytes);
	printf("  Duration          : %lld ms - %lld ns\n", duration / 1000000, duration);

	return 0;
}
