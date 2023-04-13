#pragma once
#include <assert.h>
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
