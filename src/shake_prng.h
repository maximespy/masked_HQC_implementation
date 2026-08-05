/**
 * @file shake_prng.h
 * @brief SHAKE-256 based seed expander
 */

#ifndef MSK_SHAKE_PRNG_H
#define MSK_SHAKE_PRNG_H

#include <inttypes.h>

#include "fips202.h"

typedef MSK_shake256incctx MSK_seedexpander_ctx;

typedef shake256incctx seedexpander_ctx;

/**
 * @brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[inout] [entropy_input] Input entropy bytes, an array of enlen
 * N_SHARES-shares boolean-masked bytes
 * @param[inout] [personalization_string] Personalization string, an array of
 * perlen N_SHARES-shares boolean-masked bytes
 * @param[in] enlen Byte-length of entropy string
 * @param[in] perlen Byte-length of the personalization string
 */
void MSK_shake_prng_init(MSK_uint8_t *entropy_input,
                         MSK_uint8_t *personalization_string,
                         const size_t enlen, const size_t perlen);

/**
 * @brief SHAKE-256 with incremental API and domain separation
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[in] entropy_input Pointer to input entropy bytes
 * @param[in] personalization_string Pointer to the personalization string
 * @param[in] enlen Length of entropy string in bytes
 * @param[in] perlen Length of the personalization string in bytes
 */
void shake_prng_init(uint8_t *entropy_input, uint8_t *personalization_string, uint32_t enlen, uint32_t perlen);

/**
 * @brief A SHAKE-256 based PRNG
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] [output] Output, an array of outlen N_SHARES-shares
 * boolean-masked bytes
 * @param[in] outlen Byte-length of output
 */
void MSK_shake_prng(MSK_uint8_t output[], const size_t outlen);

/**
 * @brief A SHAKE-256 based PRNG
 *
 * Derived from function SHAKE_256 in fips202.cpp
 *
 * @param[out] output Pointer to output
 * @param[in] outlen length of output in bytes
 */
void shake_prng(uint8_t *output, uint32_t outlen);

/**
 * @brief Initialize a masked SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] [ctx] Pointer to masked SHAKE-256 context
 * @param[inout] [seed] Initialization seed, an array of seedlen N_SHARES-shares
 * boolean-masked bytes (refreshed)
 * @param[in] seedlen Byte-length of seed
 */
void MSK_seedexpander_init(MSK_seedexpander_ctx *ctx, MSK_uint8_t seed[],
                           const size_t seedlen);

/**
 * @brief A SHAKE-256 based masked seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 * Squeezes Keccak state by 64-bit blocks (hardware version compatibility)
 *
 * @param[out] [output] XOF output data, an array of outlen N_SHARES-shares
 * @param[inout] [ctx] Pointer to masked SHAKE-256 context
 * boolean-masked bytes
 * @param[in] outlen Number of bytes to return
 */
void MSK_seedexpander(MSK_uint8_t output[], MSK_seedexpander_ctx *ctx,
                      const size_t outlen);

/**
 * @brief Initialise a SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 *
 * @param[out] ctx Pointer to SHAKE-256 context
 * @param[in] seed Initialization seed, an array of seedlen bytes
 * @param[in] seedlen Byte-length of seed
 */
void seedexpander_init(seedexpander_ctx *ctx, const uint8_t *const seed,
                       const size_t seedlen);

/**
 * @brief A SHAKE-256 based seed expander
 *
 * Derived from function SHAKE_256 in fips202.c
 * Squeezes Keccak state by 64-bit blocks (hardware version compatibility)
 *
 * @param[out] output XOF output data, an array of outlen bytes
 * @param[inout] ctx Pointer to SHAKE-256 context
 * @param[in] outlen Number of bytes to return
 */
void seedexpander(uint8_t *output, seedexpander_ctx *ctx, const size_t outlen);

/**
 * @brief Refresh masking of SHAKE-256 internal state
 */
void MSK_refresh_master_shake_state(void);

#endif
