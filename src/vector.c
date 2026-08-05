#include <string.h>

#include "barrett.h"
#include "vector.h"

/**
 * @brief Generate a masked 64-bits value with one single non-zero bit at
 * position pos.
 *
 * Bit positions (pos) are little endian: 0 for Least Significant.
 *
 * @param[out] [res] 64-bits N_SHARES-shares boolean-masked value with one
 * single non-zero bit at position pos
 * @param[inout] [pos] 64-bits N_SHARES-shares boolean-masked position of the
 * non-zero bit in output value (refreshed)
 */
void MSK_single_bit_mask(MSK_uint64_t ret, MSK_uint64_t pos) {
  memset(ret, 0, 8 * (N_SHARES));
  MSK_uint64_t mask = {0};
  mask[0] = 1;
  MSK_SNI_64b_inplace_refresh(pos); // no path from input to output
  MSK_uint64_t tmp;
  MSK_uint64_t mintmp;
  MSK_uint64_t tmp_or_mintmp;
  MSK_uint64_t min_tom; // - ((uint64_t)(tmp | (0 - tmp)) >> 63))
  MSK_uint64_t mask_and_tmp;

  for (size_t i = 0; i < 64; ++i) {
    // tmp = pos - i;
    MSK_64b_semi_add(tmp, pos, UINT64_MAX - i + 1);

    // tmp = 0 - (1 - ((uint64_t)(tmp | (0 - tmp)) >> 63)); // tmp = 0xF...F if
    // pos == i, else tmp = 0
    MSK_64b_neg(mintmp, tmp);
    MSK_SNI_64b_inplace_refresh(mintmp);
    MSK_SNI_64b_or(tmp_or_mintmp, tmp, mintmp);
    MSK_64b_right_shift(tmp_or_mintmp, tmp_or_mintmp, 63);

    MSK_64b_neg(min_tom, tmp_or_mintmp);

    MSK_64b_semi_add(min_tom, min_tom, 1);
    MSK_64b_neg(tmp, min_tom);

    // ret |= mask & tmp;
    MSK_SNI_64b_inplace_refresh(tmp);
    MSK_SNI_64b_and(mask_and_tmp, mask, tmp);
    MSK_SNI_64b_inplace_refresh(ret);
    MSK_SNI_64b_or(ret, ret, mask_and_tmp);
    MSK_64b_left_shift(mask, mask, 1);
  }
}

void MSK_vect_set_random_fixed_weight(MSK_uint64_t v[],
                                      MSK_seedexpander_ctx *ctx,
                                      const size_t weight) {

  MSK_uint8_t rand_u8[PARAM_OMEGA_R * 4] = {0};
  MSK_uint32_t support[PARAM_OMEGA_R] = {0};
  MSK_uint32_t index_tab[PARAM_OMEGA_R] = {0};
  MSK_uint64_t bit_tab[PARAM_OMEGA_R] = {0};
  MSK_uint32_t r;
  MSK_uint64_t pos;
  MSK_uint32_t min_it, tmp;
  MSK_uint32_t mintmp;
  MSK_uint32_t tmp_or_mintmp;
  MSK_uint64_t mask64, tmp64, btm;

  MSK_seedexpander(rand_u8, ctx, 4 * weight);

  for (size_t i = 0; i < weight; ++i) {
    for (size_t j = 0; j < 4; j++) {
      for (size_t d = 0; d < N_SHARES; d++) {
        support[i][d] ^= ((uint32_t)rand_u8[4 * i + j][d]) << (8 * j);
      }
    }
    // support is refreshed in Barrett -> no path from input to output
    MSK_barrett(r, support[i], i);
    MSK_32b_semi_add(support[i], r, i);
  }

  for (int32_t i = (weight - 1); i >= 0; i--) {
    MSK_uint32_t val_i = {0};
    val_i[0] = i;
    MSK_SNI_32b_inplace_refresh(val_i);

    for (size_t j = i + 1; j < weight; ++j) {
      MSK_uint32_t temp = {0}, temp1 = {0};
      MSK_32b_eq(temp, support[i], support[j]);
      MSK_SNI_32b_inplace_refresh(support[i]);
      MSK_32b_if(temp1, val_i, support[i], temp);
      MSK_32b_to_32b_copy(support[i], temp1);
    }
  }

  for (size_t i = 0; i < weight; ++i) {
    for (size_t d = 0; d < N_SHARES; d++) {
      pos[d] = support[i][d] & 0x3f;
    }
    MSK_32b_right_shift(index_tab[i], support[i], 6);

    MSK_single_bit_mask(bit_tab[i], pos);
  }

  // PQClean Sparse to dense conversion
  for (size_t i = 0; i < VEC_N_SIZE_64; ++i) {
    MSK_uint64_t val = {0};
    for (size_t j = 0; j < weight; ++j) {
      // tmp = (uint32_t)(i - index_tab[j]);
      MSK_32b_neg(min_it, index_tab[j]);
      MSK_32b_semi_add(tmp, min_it, i);

      // tmp = 1 ^ ((uint32_t)(tmp | (0 - tmp)) >> 31);
      MSK_32b_neg(mintmp, tmp);
      MSK_SNI_32b_inplace_refresh(tmp);
      MSK_SNI_32b_or(tmp_or_mintmp, tmp, mintmp);

      for (size_t d = 0; d < N_SHARES; d++) {
        tmp64[d] = tmp_or_mintmp[d] >> 31;
      }
      tmp64[0] ^= 1;

      // mask64 = 0 - (uint64_t)tmp;
      MSK_64b_neg(mask64, tmp64);
      // val |= (bit_tab[j] & mask64);
      MSK_SNI_64b_inplace_refresh(bit_tab[j]);
      MSK_SNI_64b_and(btm, bit_tab[j], mask64);
      MSK_SNI_64b_or(val, val, btm);
    }
    // v[i] |= val;
    MSK_SNI_64b_or(v[i], v[i], val);
  }
}

void vect_set_random(uint64_t v[VEC_N_SIZE_64], seedexpander_ctx *ctx) {
  uint8_t rand_bytes[VEC_N_SIZE_BYTES] = {0};

  seedexpander(rand_bytes, ctx, VEC_N_SIZE_BYTES);

  memcpy(v, rand_bytes, VEC_N_SIZE_BYTES);
  v[VEC_N_SIZE_64 - 1] &= BITMASK(PARAM_N, 64);
}

void MSK_vect_add(MSK_uint64_t o[], MSK_uint64_t v1[], MSK_uint64_t v2[],
                  const size_t size) {
  MSK_SNI_64b_inplace_refresh_array(v1, size); // no path from input to output
  MSK_SNI_64b_inplace_refresh_array(v2, size); // no path from input to output
  for (size_t i = 0; i < size; ++i) {
    MSK_64b_xor(o[i], v1[i], v2[i]);
  }
}

void MSK_vect_compare(MSK_uint64_t r, MSK_uint64_t v1[], MSK_uint64_t v2[],
                      const size_t size) {
  for (size_t i = 0; i < size; i++) {
    MSK_uint64_t tmp = {0};
    MSK_SNI_64b_inplace_refresh(v1[i]); // no path from input to output
    MSK_SNI_64b_inplace_refresh(v2[i]);
    MSK_64b_eq(tmp, v1[i], v2[i]);
    MSK_SNI_64b_or(r, r, tmp);
  }
}

void MSK_vect_resize(MSK_uint64_t o[], const uint32_t size_o,
                     const MSK_uint64_t v[], const uint32_t size_v) {
  // MSK_vect_resize is only called on public values
  uint64_t mask = 0x7FFFFFFFFFFFFFFF;
  size_t val = 0;
  if (size_o < size_v) {

    MSK_64b_to_64b_copy_array(o, v, VEC_N1N2_SIZE_64);

    // RP: check that
    if (size_o % 64) {
      val = 64 - (size_o % 64);
      MSK_64b_semi_and(o[VEC_N1N2_SIZE_64 - 1], o[VEC_N1N2_SIZE_64 - 1],
                       mask >> (val - 1));
    }
  } else {
    MSK_64b_to_64b_copy_array(o, v, CEIL_DIVIDE(size_v, 64));
  }
}
