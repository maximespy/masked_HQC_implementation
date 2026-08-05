#include "gf2x.h"
#include <string.h>

/**
 * @brief Masked Karatsuba addition of halves
 *
 * @param[out] [alh] Sum of low and high halves of a, array of size_h 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] [a] Input polynomial, array of size_h + size_l 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] size_h Size of high half of Karatsuba multiplication operands
 * (size / 2)
 * @param[in] size_l Size of low half of Karatsuba multiplication operands
 * ((size + 1) / 2)
 */
static void MSK_karatsuba_add_halves(MSK_uint64_t alh[], const MSK_uint64_t a[],
                                     const size_t size_l, const size_t size_h) {
  for (size_t i = 0; i < size_h; i++) {
    MSK_64b_xor(alh[i], a[i], a[i + size_l]);
  }

  if (size_h < size_l) {
    MSK_64b_to_64b_copy(alh[size_h], a[size_h]);
  }
}

/**
 * @brief Masked Karatsuba final addition
 *
 * Compute o = o + tmp1 * (1 + 2^size_l) + tmp2
 *
 * @param[inout] [o] Final sum, array of 3 * size_l 64-bits N_SHARES-shares
 * boolean-masked values
 * @param[inout] [tmp1] First partial sum, array of 2 * size_l 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[inout] [tmp2] Second partial sum, array of 2 * size_h 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] size_h Size of high half of Karatsuba multiplication operands(size
 * / 2)
 * @param[in] size_l Size of low half of Karatsuba multiplication operands((size
 * + 1) / 2)
 */
static void MSK_karatsuba_add_final(MSK_uint64_t o[], MSK_uint64_t tmp1[],
                                    MSK_uint64_t tmp2[], size_t size_l,
                                    size_t size_h) {
#ifdef MIMOSNI
  MSK_SNI_64b_inplace_refresh_array(tmp1, (2 * size_l));
  MSK_SNI_64b_inplace_refresh_array(o, (2 * size_l));
  MSK_SNI_64b_inplace_refresh_array(tmp2, (2 * size_h));
#endif
  for (size_t i = 0; i < (2 * size_l); i++) {
    MSK_64b_xor(tmp1[i], tmp1[i], o[i]);
  }

  for (size_t i = 0; i < (2 * size_h); i++) {
    MSK_64b_xor(tmp1[i], tmp1[i], tmp2[i]);
  }

  for (size_t i = 0; i < (2 * size_l); i++) {
    MSK_64b_xor(o[i + size_l], o[i + size_l], tmp1[i]);
  }
}

/**
 * @brief Masked 64-bits product
 *
 * Compute x = a * b
 *
 * @param[inout] [x] Product result, array of two 64-bits N_SHARES-shares
 * boolean-masked values
 * @param[inout] [a] First operand, 64-bits N_SHARES-shares boolean-masked
 * values (refreshed)
 * @param[inout] [b] Second operand, 64-bits N_SHARES-shares boolean-masked
 * values (refreshed)
 */
void MSK_64b_mul(MSK_uint64_t x[2], MSK_uint64_t a, MSK_uint64_t b) {
  MSK_SNI_64b_inplace_refresh(a);
  memset(x, 0, (N_SHARES) * 2 * 8);
  for (size_t i = 0; i < 64; i++) {
    MSK_uint64_t t;

    for (int d = 0; d < N_SHARES; d++) {
      t[d] = (((a[d]) >> i) & 1) * UINT64_MAX;
    }
    MSK_uint64_t u;
    MSK_SNI_64b_and(u, t, b);
    MSK_SNI_64b_inplace_refresh(a);
    MSK_SNI_64b_inplace_refresh(b);

    if (i > 0) {
      for (int d = 0; d < N_SHARES; d++) {
        x[1][d] ^= (u[d] >> (64 - i));
      }
      MSK_SNI_64b_inplace_refresh(u);
      for (int d = 0; d < N_SHARES; d++) {
        x[0][d] ^= (u[d] & (((uint64_t)(1) << (64 - i)) - 1)) << i;
      }
    } else {
      MSK_64b_to_64b_copy(x[0], u);
    }
    MSK_SNI_64b_inplace_refresh(x[0]);
  }
}

/**
 * Masked Karatsuba multiplication of a and b,
 * Implementation inspired from the NTL library.
 *
 * @param[out] [o] Product polynomial (a * b), array of 2*size 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[inout] [a] First polynomial, array of size 64-bits N_SHARES-shares
 * boolean-masked values (refreshed)
 * @param[inout] [b] Second polynomial, array of size 64-bits N_SHARES-shares
 * boolean-masked values (refreshed)
 * @param[inout] [stack] Temporary storage, array of 8*size 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] Length of polynomials in 64-bits dwords
 */
static void MSK_karatsuba(MSK_uint64_t o[], MSK_uint64_t a[], MSK_uint64_t b[],
                          MSK_uint64_t stack[], const size_t size) {
  // Stop recurse case
  if (size == 1) {
    MSK_64b_mul(o, a[0], b[0]);
    return;
  }

  // No need to mask
  size_t size_h = size / 2;
  size_t size_l = (size + 1) / 2;

  // High halves
  MSK_uint64_t *ah = a + size_l;
  MSK_uint64_t *bh = b + size_l;

  // Temporary
  MSK_uint64_t *alh = stack;
  MSK_uint64_t *blh = alh + size_l;
  MSK_uint64_t *tmp1 = blh + size_l;
  MSK_uint64_t *tmp2 = o + 2 * size_l;
  MSK_uint64_t *new_stack = stack + 4 * size_l;

  // Product of low halves (al * bl)
  MSK_karatsuba(o, a, b, new_stack, size_l);

  MSK_SNI_64b_inplace_refresh_array(new_stack, size);

  // Product of high halves (ah * bh)
  MSK_karatsuba(tmp2, ah, bh, new_stack, size_h);

  MSK_SNI_64b_inplace_refresh_array(a, size_l);
  MSK_SNI_64b_inplace_refresh_array(b, size_l);

  // Additions of halves (alh = ah + al, blh = bh + bl)
  MSK_karatsuba_add_halves(alh, a, size_l, size_h);
  MSK_karatsuba_add_halves(blh, b, size_l, size_h);

  MSK_SNI_64b_inplace_refresh_array(new_stack, size);

  // Product of sums of halves ((ah + al) * (bh + bl))
  MSK_karatsuba(tmp1, alh, blh, new_stack, size_l);

  // Final additions
  MSK_karatsuba_add_final(o, tmp1, tmp2, size_l, size_h);
}

/**
 * @brief Masked modular reduction
 *
 * @param[out] [out] Reduced vector, array of VEC_N_SIZE_64 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] [x] Raw vector, array of 2*VEC_N_SIZE_64 64-bits N_SHARES-shares
 * boolean-masked values
 */
void MSK_reduce(MSK_uint64_t out[VEC_N_SIZE_64],
                MSK_uint64_t x[2 * VEC_N_SIZE_64]) {
  uint8_t s = PARAM_N & 0x3F;

  MSK_64b_to_64b_copy_array(out, x, VEC_N_SIZE_64);

  for (int d = 0; d < N_SHARES; d++) {
    for (int i = 0; i < VEC_N_SIZE_64; i++) {
      out[i][d] ^= x[i + VEC_N_SIZE_64 - 1][d] >> s;
      out[i][d] ^= x[i + VEC_N_SIZE_64][d] << (64 - s);
    }
    out[VEC_N_SIZE_64 - 1][d] &= RED_MASK;
  }
}

void MSK_vect_mul(MSK_uint64_t o[VEC_N_SIZE_64], MSK_uint64_t v1[VEC_N_SIZE_64],
                  MSK_uint64_t v2[VEC_N_SIZE_64]) {
  MSK_uint64_t stack[VEC_N_SIZE_64 << 3] = {0};
  MSK_uint64_t o_karat[VEC_N_SIZE_64 << 1] = {0};

  MSK_karatsuba(o_karat, v1, v2, stack, VEC_N_SIZE_64);
  MSK_reduce(o, o_karat);
}
