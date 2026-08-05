/**
 * @file prng.h
 * @brief Configurable wrapper around a Pseudo Random Number Generator (PRNG)
 */

#ifndef MSK_PRNG_H
#define MSK_PRNG_H

#include <inttypes.h>
#include <stdlib.h>

/**
 * @brief Enumeration of PRNG types.
 */
typedef enum {
  PRNG_ZERO_FAST,  /**< Constant 0, use for performance evaluations */
  PRNG_ZERO,       /**< Constant 0, use as unmasked reference for T-tests */
  PRNG_XOSHIRO,    /**< XOSHIRO256 */
  PRNG_SHAKE,      /**< SHAKE-256 */
  PRNG_DEVURANDOM, /**< /dev/urandom */
#if STM32F4XX_TRNG != 0
  PRNG_STM32F4XX_TRNG /**< STM32 F4xx series True Random Number Generator (TRNG)
                       */
#endif
} prng_type_t;

/**
 * @brief Enumeration of PRNG modes.
 */
typedef enum {
  PRNG_AUTO_REFILL, /**< Automatically refill pool2 from PRNG when empty */
  PRNG_LOOP,        /**< Loop over old pool2 content when reaching its end */
  PRNG_CRASH /**< Crash when trying to read 8 new random bytes from empty pool2
              */
} prng_mode_t;

/**
 * @brief Wrapper initialization
 * @param[in] type Type of PRNG to use
 * @param[in] pool2_size Size (number of uint64_t) of second level pool of
 * randomness (pool2)
 */
void prng_init(const prng_type_t type, const size_t pool2_size,
               const prng_mode_t mode);

/**
 * @brief Fill pools from PRNG such that they contain 16+8*pool2_size fresh
 * random bytes.
 */
void prng_fill(void);

/**
 * @brief Return total number of drawn bytes minus the number of unused random
 * bytes in pool
 *
 * @return Total number of drawn bytes minus the number of unused random bytes
 * in pool, that is, random bytes actually used
 */
uint64_t prng_amount(void);

/**
 * @brief Close PRNG
 */
void prng_close(void);

/**
 * @brief Return 8 bits random number
 *
 * @return A 8 bits random number
 */
uint8_t next_8(void);

/**
 * @brief Return 16 bits random number
 *
 * @return A 16 bits random number
 */
uint16_t next_16(void);

/**
 * @brief Return 32 bits random number
 *
 * @return A 32 bits random number
 */
uint32_t next_32(void);

/**
 * @brief Return 64 bits random number
 *
 * @return A 64 bits random number
 */
uint64_t next_64(void);

#endif
