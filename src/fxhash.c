// SPDX-License-Identifier: MIT
#include "fxhash.h"

uintptr_t fxhash_impl (unsigned char const *data,
                       size_t               len) asm("fxhash");

uintptr_t fxhash_impl (unsigned char const *data,
                       size_t               len)
{
	return fxhash(data, len);
}
