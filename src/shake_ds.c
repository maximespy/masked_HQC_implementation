/**
 * @file shake_ds.c
 * @brief Implementation SHAKE-256 with incremental API and domain separation
 */

#include "shake_ds.h"

void MSK_shake256_512_ds(MSK_uint8_t output[], MSK_shake256incctx *ctx,
                         MSK_uint8_t *input, const size_t inlen,
                         const uint8_t domain) {
  /* Init context */
  MSK_shake256_inc_init(ctx);

#ifdef MIMOSNI
  MSK_SNI_8b_inplace_refresh_array(input, inlen);
#endif
  /* Absorb input */
  MSK_shake256_inc_absorb(ctx, input, inlen);

  /* Absorb domain separation byte (that does not need masking) */
  MSK_uint8_t dom = {0};
  dom[0] = domain;
  MSK_shake256_inc_absorb(ctx, &dom, 1);

  /* Finalize */
  MSK_shake256_inc_finalize(ctx);

  /* Squeeze output */
  MSK_shake256_inc_squeeze(output, ctx, 512 / 8);
#ifdef MIMOSNI
  MSK_SNI_8b_inplace_refresh_array(output, 512 / 8);
#endif
}
