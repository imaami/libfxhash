// SPDX-License-Identifier: MIT
#ifndef LIBFXHASH_H_
#define LIBFXHASH_H_

#ifndef __cplusplus
# include <stddef.h>
# include <stdint.h>
# define NS(x)
#else // __cplusplus
# include <cstddef>
# include <cstdint>
# define NS(x) x::
extern "C" {
#endif // __cplusplus

__attribute__((visibility("default")))
extern NS(::std) uintptr_t fxhash (unsigned char const *data,
                                   NS(::std) size_t     len);

#undef NS

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIBFXHASH_H_
