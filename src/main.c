// SPDX-License-Identifier: MIT
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fxhash/fxhash.h"

#if UINTPTR_MAX == UINT64_MAX
# define FMT_PAD "%016"
#else
# define FMT_PAD "%08"
#endif

int main (int    argc,
          char **argv)
{
	if (!argv)
		return EXIT_FAILURE;

	for (int i = 0; ++i < argc && argv[i]; ) {
		printf(FMT_PAD PRIxPTR "\n",
		       fxhash((unsigned char *)argv[i], strlen(argv[i])));
	}

	return EXIT_SUCCESS;
}
