/**
 * @file shake_prng.c
 * @brief Implementation of SHAKE-256 based seed expander
 */

#include <string.h>

#include "domains.h"
#include "shake_prng.h"

/**
 * @brief Masked SHAKE-256 context used for HQC seed expansion
 */
MSK_shake256incctx MSK_shake_prng_ctx;

/**
 * @brief Unmasked SHAKE-256 state used for salt sampling
 */
shake256incctx shake_prng_state;

void MSK_shake_prng_init(MSK_uint8_t *entropy_input,
                         MSK_uint8_t *personalization_string,
                         const size_t enlen, const size_t perlen) {
  MSK_uint8_t tmpdom = {0};
  tmpdom[0] = PRNG_DOMAIN;

  MSK_shake256_inc_init(&MSK_shake_prng_ctx);
  MSK_shake256_inc_absorb(&MSK_shake_prng_ctx, entropy_input, enlen);
  MSK_shake256_inc_absorb(&MSK_shake_prng_ctx, personalization_string, perlen);
  MSK_shake256_inc_absorb(&MSK_shake_prng_ctx, &tmpdom, 1);
  MSK_shake256_inc_finalize(&MSK_shake_prng_ctx);
}

void shake_prng_init(uint8_t *entropy_input, uint8_t *personalization_string, uint32_t enlen, uint32_t perlen) {
    uint8_t domain = PRNG_DOMAIN;
    shake256_inc_init(&shake_prng_state);
    shake256_inc_absorb(&shake_prng_state, entropy_input, enlen);
    shake256_inc_absorb(&shake_prng_state, personalization_string, perlen);
    shake256_inc_absorb(&shake_prng_state, &domain, 1);
    shake256_inc_finalize(&shake_prng_state);
}

void MSK_shake_prng(MSK_uint8_t output[], const size_t outlen) {
  MSK_shake256_inc_squeeze(output, &MSK_shake_prng_ctx, outlen);
}

void shake_prng(uint8_t *output, uint32_t outlen) {
    shake256_inc_squeeze(output, &shake_prng_state, outlen);
}

void MSK_seedexpander_init(MSK_seedexpander_ctx *ctx, MSK_uint8_t seed[],
                           const size_t seedlen) {
  MSK_uint8_t tmpdom = {0};
  tmpdom[0] = SEEDEXPANDER_DOMAIN;

  MSK_shake256_inc_init(ctx);
#ifdef MIMOSNI
  MSK_SNI_8b_inplace_refresh_array(seed, seedlen);
#endif
  MSK_shake256_inc_absorb(ctx, seed, seedlen);
  MSK_shake256_inc_absorb(ctx, &tmpdom, 1);
  MSK_shake256_inc_finalize(ctx);
}

void seedexpander_init(seedexpander_ctx *ctx, const uint8_t *const seed,
                       const size_t seedlen) {
  uint8_t domain = SEEDEXPANDER_DOMAIN;
  shake256_inc_init(ctx);
  shake256_inc_absorb(ctx, seed, seedlen);
  shake256_inc_absorb(ctx, &domain, 1);
  shake256_inc_finalize(ctx);
}

void MSK_seedexpander(MSK_uint8_t *output, MSK_seedexpander_ctx *ctx,
                      const size_t outlen) {
  const size_t bsize = sizeof(uint64_t);
  const size_t remainder = outlen % bsize;
  MSK_uint8_t tmp[sizeof(uint64_t)] = {{0}};

  MSK_shake256_inc_squeeze(output, ctx, outlen - remainder);

  if (remainder != 0) {
    MSK_shake256_inc_squeeze(tmp, ctx, bsize);

    output += outlen - remainder;
    MSK_8b_to_8b_copy_array(output, tmp, remainder);
  }
}

void seedexpander(uint8_t *output, seedexpander_ctx *ctx, const size_t outlen) {
  const size_t bsize = sizeof(uint64_t);
  const size_t remainder = outlen % bsize;
  uint8_t tmp[sizeof(uint64_t)];
  shake256_inc_squeeze(output, ctx, outlen - remainder);

  if (remainder != 0) {
    shake256_inc_squeeze(tmp, ctx, bsize);

    output += outlen - remainder;
    for (size_t i = 0; i < remainder; ++i) {
      output[i] = tmp[i];
    }
  }
}

void MSK_refresh_master_shake_state(void) {
  MSK_SNI_64b_inplace_refresh_array(MSK_shake_prng_ctx.state, 25);
}
