/**
 * @file xoshiro256starstar.h
 * @brief XOSHIRO256** pseudo random numbers generation
 */

#ifndef MSK_XOSHIRO_H
#define MSK_XOSHIRO_H

#include <inttypes.h>

/**
 * @brief Get next 64 bits from XOSHIRO256** PRNG
 */
uint64_t xoshiro_next(void);

/**
 * @brief Seed XOSHIRO256** PRNG with /dev/urandom
 * @return 1 on success, 0 on failure
 */
int seed_xoshiro(void);

#endif
