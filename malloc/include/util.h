#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ft_assert(left, operator, right) \
	{                                    \
		assert(left operator right);     \
	}

// crude macro TODO: improve
#define EXIT_WITH_ERROR(...)                    \
	do                                          \
	{                                           \
		fprintf(stderr, "Function: ");          \
		fprintf(stderr, __func__);              \
		fprintf(stderr, "(...)\nFile:     ./"); \
		fprintf(stderr, __FILE__);              \
		fprintf(stderr, ":");                   \
		fprintf(stderr, "%d", __LINE__);        \
		fprintf(stderr, "\nMessage:  ");        \
		fprintf(stderr, __VA_ARGS__);           \
		fprintf(stderr, "\n");                  \
		exit(EXIT_FAILURE);                     \
	} while (0)

size_t nearest_multiple_of(size_t number, size_t multiple);
