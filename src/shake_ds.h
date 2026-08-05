/**
 * @file shake_ds.h
 * @brief SHAKE-256 with incremental API and domain separation
 */

#ifndef MSK_SHAKE_DS_H
#define MSK_SHAKE_DS_H

#include <inttypes.h>

#include "mask.h"
#include "shake_prng.h"

/**
 * @brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 * Initialize the SHAKE-256 state, absord the input, absorb the domain
 * separation byte, finalize, and squeeze 64 bytes (512 bits) to output.
 *
 * @param[out] [output] Output bytes, array of 64 N_SHARES-shares boolean-masked
 * bytes
 * @param[inout] [state] Masked SHAKE-256 state
 * @param[inout] [input] Input bytes, array of inlen N_SHARES-shares
 * boolean-masked bytes (refreshed)
 * @param[in] inlen Byte-length of input
 * @param[in] domain Domain separation byte
 */
void MSK_shake256_512_ds(MSK_uint8_t output[], MSK_shake256incctx *ctx,
                         MSK_uint8_t *input, const size_t inlen,
                         const uint8_t domain);

#endif
