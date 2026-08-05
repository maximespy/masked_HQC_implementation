/**
 * @file fips202.h
 * @brief Masked version of FIPS202 (SHA-3)
 *
 * Based on the public domain implementation in
 * crypto_hash/keccakc512/simple/ from http://bench.cr.yp.to/supercop.html
 * by Ronny Van Keer
 * and the public domain "TweetFips202" implementation
 * from https://twitter.com/tweetfips202
 * by Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe
 */

#ifndef MSK_FIPS202_H
#define MSK_FIPS202_H

#include <inttypes.h>

#include "mask.h"

#define SHAKE256_RATE 136

/**
 * @brief Masked context for SHAKE-256 incremental API
 */
typedef struct {
  MSK_uint64_t
      state[25]; /**< Incremental state as 25 N_SHARES-shares 64-bits values */
  size_t nsqueezed; /**< Unmasked number of absorbed bytes that have not been
                       permuted, or not-yet-squeezed bytes */
} MSK_shake256incctx;

/**
 * @brief Unmasked context for SHAKE-256 incremental API
 */
typedef struct {
  uint64_t state[25]; /**< Incremental state as 25 64-bits values */
  size_t nsqueezed;   /**< Number of absorbed bytes that have not been permuted,
                         or not-yet-squeezed bytes */
} shake256incctx;

/**
 * @brief Initializes the incremental SHAKE-256 masked context to zero
 *
 * @param[inout] [ctx] Incremental SHAKE-256 masked context
 */
void MSK_shake256_inc_init(MSK_shake256incctx *ctx);

/**
 * @brief Incremental SHAKE-256 masked absorb
 *
 * Preceded by MSK_shake256_inc_init, succeeded by MSK_shake256_inc_finalize
 *
 * @param[inout] [ctx] Pointer to incremental SHAKE-256 masked context
 * @param[in] [input] Input to be absorbed into state, array of N_SHARES-shares
 * boolean-masked bytes
 * @param[in] inlen Byte-length of input
 */
void MSK_shake256_inc_absorb(MSK_shake256incctx *ctx, const MSK_uint8_t *input,
                             const size_t inlen);

/**
 * @brief Incremental SHAKE-256 masked squeeze
 *
 * @param[out] [output] Array of N_SHARES-shares boolean-masked bytes
 * @param[inout] [ctx] Pointer to incremental SHAKE-256 context
 * @param[in] outlen Number of bytes to squeeze
 */
void MSK_shake256_inc_squeeze(MSK_uint8_t output[], MSK_shake256incctx *ctx,
                              const size_t outlen);

/**
 * @brief Incremental SHAKE-256 masked finalization
 *
 * @param[inout] [ctx] Pointer to incremental SHAKE-256 masked context
 */
void MSK_shake256_inc_finalize(MSK_shake256incctx *ctx);

/**
 * @brief Initializes the incremental SHAKE-256 context to zero
 *
 * @param[inout] ctx Incremental SHAKE-256 context
 */
void shake256_inc_init(shake256incctx *ctx);

/**
 * @brief Incremental SHAKE-256 absorb
 *
 * Preceded by shake256_inc_init, succeeded by shake256_inc_finalize
 *
 * @param[inout] ctx Pointer to incremental SHAKE-256 context
 * @param[in] input Input to be absorbed into state
 * @param[in] inlen Byte-length of input
 */
void shake256_inc_absorb(shake256incctx *ctx, const uint8_t *input,
                         const size_t inlen);

/**
 * @brief Incremental SHAKE-256 squeeze
 *
 * @param[out] output Output bytes
 * @param[inout] ctx Pointer to incremental SHAKE-256 context
 * @param[in] outlen Number of bytes to squeeze
 */
void shake256_inc_squeeze(uint8_t *output, shake256incctx *ctx,
                          const size_t outlen);

/**
 * @brief Incremental SHAKE-256 finalization
 *
 * @param[inout] ctx Pointer to incremental SHAKE-256 context
 */
void shake256_inc_finalize(shake256incctx *ctx);

#endif
