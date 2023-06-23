// SPDX-License-Identifier: MIT
/** @file
 * @brief Header-only C implementation of the rustc-hash algorithm.
 */
#ifndef FXHASH_H_
#define FXHASH_H_

#ifndef __cplusplus
# include <limits.h>
# include <stddef.h>
# include <stdint.h>
# define STD
#else // __cplusplus
# include <climits>
# include <cstddef>
# include <cstdint>
# define STD ::std::
extern "C" {
#endif // __cplusplus

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
# define unpack_u64(b) ((STD uint64_t)(b)[0]        | ((STD uint64_t)(b)[1] <<  8) \
                     | ((STD uint64_t)(b)[2] << 16) | ((STD uint64_t)(b)[3] << 24) \
                     | ((STD uint64_t)(b)[4] << 32) | ((STD uint64_t)(b)[5] << 40) \
                     | ((STD uint64_t)(b)[6] << 48) | ((STD uint64_t)(b)[7] << 56))
# define unpack_u32(b) ((STD uint32_t)(b)[0]        | ((STD uint32_t)(b)[1] <<  8) \
                     | ((STD uint32_t)(b)[2] << 16) | ((STD uint32_t)(b)[3] << 24))
# define unpack_u16(b) ((STD uint16_t)(b)[0]        | ((STD uint16_t)(b)[1] <<  8))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
# define unpack_u64(b) ((STD uint64_t)(b)[7]        | ((STD uint64_t)(b)[6] <<  8) \
                     | ((STD uint64_t)(b)[5] << 16) | ((STD uint64_t)(b)[4] << 24) \
                     | ((STD uint64_t)(b)[3] << 32) | ((STD uint64_t)(b)[2] << 40) \
                     | ((STD uint64_t)(b)[1] << 48) | ((STD uint64_t)(b)[0] << 56))
# define unpack_u32(b) ((STD uint32_t)(b)[3]        | ((STD uint32_t)(b)[2] <<  8) \
                     | ((STD uint32_t)(b)[1] << 16) | ((STD uint32_t)(b)[0] << 24))
# define unpack_u16(b) ((STD uint16_t)(b)[1]        | ((STD uint16_t)(b)[0] <<  8))
#else
# error "Unsupported byte order"
#endif

#if UINTPTR_MAX == UINT64_MAX
# define unpack_uptr unpack_u64
# define K           (STD uint64_t)(UINT64_MAX / 3.141592653589793238462643383279502884L + .5)
# ifdef __clang__
#  define ROL5(x)    __builtin_rotateleft64((x), 5)
# endif // __clang__
#elif UINTPTR_MAX == UINT32_MAX
# define unpack_uptr unpack_u32
# define K           (STD uint32_t)(UINT32_MAX / 1.618033988749894848204586834365 + .5)
# ifdef __clang__
#  define ROL5(x)    __builtin_rotateleft32((x), 5)
# endif // __clang__
#else
# error "Unsupported pointer size"
#endif

#ifndef ROL5
# define ROL5(x) (((x) << 5) | ((x) >> ((sizeof(x) * CHAR_BIT) - 5u)))
#endif // ROL5

/**
 * @brief Mix a value into the hash.
 *
 * This macro mixes an input value into the hash by left-rotating the
 * current hash, performing a bitwise XOR of the rotated hash and the
 * input, and finally multiplying the result by the constant @ref K.
 *
 * The hash is assumed to be a @c uintptr_t, the input value is cast
 * to a @c uintptr_t before being mixed into the hash, and the value
 * of @ref K depends on `sizeof(uintptr_t)`.
 *
 * @param h_ The current hash value. Should be a @c uintptr_t.
 * @param v_ The value to mix in. Should be an integral type.
 *
 * @return The updated hash value.
 */
#define fxhash_add(h_, v_) ((ROL5(h_) ^ (STD uintptr_t)(v_)) * K)

/**
 * @brief Calculate the hash value of a byte array.
 *
 * This function calculates the hash value of a byte array using the
 * fxhash algorithm, specifically the same implementation as rustc.
 *
 * @warning This function only supports little-endian and big-endian
 * systems. Sorry, PDP-11 enthusiasts!
 *
 * @param data A pointer to the byte array.
 * @param size The size of the byte array in bytes.
 * @return The hash value of the byte array.
 *
 * @see fxhash_add
 * @see unpack_uptr
 */
static inline uintptr_t fxhash(unsigned char const* data, size_t size);
__attribute__((always_inline))
static inline STD uintptr_t fxhash (unsigned char const *data,
                                    STD size_t           size)
{
	STD uintptr_t hash = 0;

	while (size >= sizeof(STD uintptr_t)) {
		hash = fxhash_add(hash, unpack_uptr(data));
		data += sizeof(STD uintptr_t);
		size -= sizeof(STD uintptr_t);
	}

#if UINTPTR_MAX > UINT32_MAX
	if (size >= sizeof(STD uint32_t)) {
		hash = fxhash_add(hash, unpack_u32(data));
		data += sizeof(STD uint32_t);
		size -= sizeof(STD uint32_t);
	}
#endif // UINTPTR_MAX > UINT32_MAX

#if UINTPTR_MAX > UINT16_MAX
	if (size >= sizeof(STD uint16_t)) {
		hash = fxhash_add(hash, unpack_u16(data));
		data += sizeof(STD uint16_t);
		size -= sizeof(STD uint16_t);
	}
#endif // UINTPTR_MAX > UINT16_MAX

#if UINTPTR_MAX > UINT8_MAX
	if (size >= sizeof(STD uint8_t))
		hash = fxhash_add(hash, *data);
#endif // UINTPTR_MAX > UINT8_MAX

	return hash;
}

#undef K
#undef ROL5
#undef STD
#undef fxhash_add
#undef unpack_uptr
#undef unpack_u16
#undef unpack_u32
#undef unpack_u64

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FXHASH_H_
