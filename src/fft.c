#include <string.h>

#include "fft.h"
#include "gf.h"

//==============================================================================//
//                Common functions that do not need to be protected //
//==============================================================================//

/**
 * @brief Compute basis of betas (omitting 1) used in additive FFT and its
 * transpose NO NEED TO MASK
 * @param[out] betas Array of PARAM_M-1 16-bits values
 */
static void compute_fft_betas(uint16_t betas[PARAM_M - 1]) {
  for (size_t i = 0; i < PARAM_M - 1; i++) {
    betas[i] = 1 << (PARAM_M - 1 - i);
  }
}

/**
 * @brief Compute subset sums of given set
 *
 * The array subset_sums is such that its ith element is
 * the subset sum of the set elements given by the binary form of i.
 * NO  NEED TO MASK
 *
 * @param[out] subset_sums Subset sums, array of 2^set_size 16-bits values
 * @param[in] set Array of set_size 16-bits values
 * @param[in] set_size Size of the array set
 */
static void compute_subset_sums(uint16_t subset_sums[], const uint16_t set[],
                                const uint8_t set_size) {
  uint16_t i, j;
  subset_sums[0] = 0;

  for (i = 0; i < set_size; i++) {
    for (j = 0; j < (1 << i); j++) {
      subset_sums[(1 << i) + j] = set[i] ^ subset_sums[j];
    }
  }
}

//==============================================================================//
//                              Masked functions //
//==============================================================================//

/**
 * @brief Compute radix conversion of polynomial f in GF(2^m)[x]
 *
 * Compute f0 and f1 such that f(x) = f0(x^2-x) + x.f1(x^2-x)
 * as proposed by Bernstein, Chou and Schwabe:
 * https://binary.cr.yp.to/mcbits-20130616.pdf
 *
 * @param[out] [f0] Array of |f]/2 16-bits N_SHARES-shares boolean-masked values
 * @param[out] [f1] Array of |f]/2 16-bits N_SHARES-shares boolean-masked values
 * @param[inout] [f] Array of 16-bits N_SHARES-shares boolean-masked values, |f|
 * is a power of 2 (refreshed)
 * @param[in] m_f Rounded-up log2 of |f| (2^m_f is smallest power of 2 greater
 * or equal number of coefficients of f)
 */
void MSK_radix_big(MSK_uint16_t f0[], MSK_uint16_t f1[], MSK_uint16_t f[],
                   const uint32_t m_f);

/**
 * @brief Compute radix conversion of polynomial f in GF(2^m)[x]
 *
 * Compute f0 and f1 such that f(x) = f0(x^2-x) + x.f1(x^2-x)
 * as proposed by Bernstein, Chou and Schwabe:
 * https://binary.cr.yp.to/mcbits-20130616.pdf
 *
 * @param[out] [f0] Array of |f]/2 16-bits N_SHARES-shares boolean-masked values
 * @param[out] [f1] Array of |f]/2 16-bits N_SHARES-shares boolean-masked values
 * @param[inout] [f] Array of 16-bits N_SHARES-shares boolean-masked values, |f|
 * is a power of 2 (refreshed)
 * @param[in] m_f Rounded-up log2 of |f| (2^m_f is smallest power of 2 greater
 * or equal number of coefficients of f)
 */
void MSK_radix(MSK_uint16_t f0[], MSK_uint16_t f1[], MSK_uint16_t f[],
               const uint32_t m_f) {
  // no path from input to output
  MSK_SNI_16b_inplace_refresh_array(f, 1 << m_f);
  switch (m_f) {
  case 4:
    // f0[4] = f[8] ^ f[12];
    MSK_16b_xor(f0[4], f[8], f[12]);

    // f0[6] = f[12] ^ f[14];
    MSK_SNI_16b_inplace_refresh(f[12]);
    MSK_16b_xor(f0[6], f[12], f[14]);

    // f0[7] = f[14] ^ f[15];
    MSK_SNI_16b_inplace_refresh(f[14]);
    MSK_16b_xor(f0[7], f[14], f[15]);

    // f1[5] = f[11] ^ f[13];
    MSK_16b_xor(f1[5], f[11], f[13]);

    // f1[6] = f[13] ^ f[14];
    MSK_SNI_16b_inplace_refresh(f[13]);
    MSK_SNI_16b_inplace_refresh(f[14]);
    MSK_16b_xor(f1[6], f[13], f[14]);

    // f1[7] = f[15];
    MSK_16b_to_16b_copy(f1[7], f[15]);
    MSK_SNI_16b_inplace_refresh(f1[7]);

    // f0[5] = f[10] ^ f[12] ^ f1[5];
    MSK_SNI_16b_inplace_refresh(f[12]);
    MSK_16b_xor3(f0[5], f[10], f[12], f1[5]);

    // f1[4] = f[9] ^ f[13] ^ f0[5];
    MSK_SNI_16b_inplace_refresh(f[13]);
    MSK_16b_xor3(f1[4], f[9], f[13], f0[5]);

    // f0[0] = f[0];
    MSK_16b_to_16b_copy(f0[0], f[0]);

    // f1[3] = f[7] ^ f[11] ^ f[15];
    MSK_SNI_16b_inplace_refresh(f[11]);
    MSK_SNI_16b_inplace_refresh(f[15]);
    MSK_16b_xor3(f1[3], f[7], f[11], f[15]);

    // f0[3] = f[6] ^ f[10] ^ f[14] ^ f1[3];
    MSK_SNI_16b_inplace_refresh(f[10]);
    MSK_SNI_16b_inplace_refresh(f[14]);
    MSK_16b_xor4(f0[3], f[6], f[10], f[14], f1[3]);

    // f0[2] = f[4] ^ f0[4] ^ f0[3] ^ f1[3];
    MSK_SNI_16b_inplace_refresh(f1[3]);
    MSK_16b_xor4(f0[2], f[4], f0[4], f0[3], f1[3]);

    // f1[1] = f[3] ^ f[5] ^ f[9] ^ f[13] ^ f1[3];
    MSK_SNI_16b_inplace_refresh(f[9]);
    MSK_SNI_16b_inplace_refresh(f[13]);
    MSK_SNI_16b_inplace_refresh(f1[3]);
    MSK_16b_xor5(f1[1], f[3], f[5], f[9], f[13], f1[3]);

    // f1[2] = f[3] ^ f1[1] ^ f0[3];
    MSK_SNI_16b_inplace_refresh(f[3]);
    MSK_SNI_16b_inplace_refresh(f0[3]);
    MSK_16b_xor3(f1[2], f[3], f1[1], f0[3]);

    // f0[1] = f[2] ^ f0[2] ^ f1[1];
    MSK_SNI_16b_inplace_refresh(f1[1]);
    MSK_16b_xor3(f0[1], f[2], f0[2], f1[1]);

    // f1[0] = f[1] ^ f0[1];
    MSK_16b_xor(f1[0], f[1], f0[1]);
    break;

  case 3:
    // f0[0] = f[0];
    MSK_16b_to_16b_copy(f0[0], f[0]);
    // f0[2] = f[4] ^ f[6];
    MSK_16b_xor(f0[2], f[4], f[6]);

    // f0[3] = f[6] ^ f[7];
    MSK_SNI_16b_inplace_refresh(f[6]);
    MSK_16b_xor(f0[3], f[6], f[7]);

    // f1[1] = f[3] ^ f[5] ^ f[7];
    MSK_SNI_16b_inplace_refresh(f[7]);
    MSK_16b_xor3(f1[1], f[3], f[5], f[7]);

    // f1[2] = f[5] ^ f[6];
    MSK_SNI_16b_inplace_refresh(f[5]);
    MSK_SNI_16b_inplace_refresh(f[6]);
    MSK_16b_xor(f1[2], f[5], f[6]);

    // f1[3] = f[7];
    MSK_16b_to_16b_copy(f1[3], f[7]);
    MSK_SNI_16b_inplace_refresh(f1[3]);

    // f0[1] = f[2] ^ f0[2] ^ f1[1];
    MSK_16b_xor3(f0[1], f[2], f0[2], f1[1]);

    // f1[0] = f[1] ^ f0[1];
    MSK_16b_xor(f1[0], f[1], f0[1]);
    break;

  case 2:
    // f0[0] = f[0];
    MSK_16b_to_16b_copy(f0[0], f[0]);
    // f0[1] = f[2] ^ f[3];
    MSK_16b_xor(f0[1], f[2], f[3]);
    // f1[0] = f[1] ^ f0[1];
    MSK_16b_xor(f1[0], f[1], f0[1]);
    // f1[1] = f[3];
    MSK_16b_to_16b_copy(f1[1], f[3]);
    break;

  case 1:
    // f0[0] = f[0];
    MSK_16b_to_16b_copy(f0[0], f[0]);
    // f1[0] = f[1];
    MSK_16b_to_16b_copy(f1[0], f[1]);
    break;

  default:
    MSK_radix_big(f0, f1, f, m_f);
    break;
  }
}

void MSK_radix_big(MSK_uint16_t f0[], MSK_uint16_t f1[], MSK_uint16_t f[],
                   const uint32_t m_f) {
  // no path from input to output
  MSK_SNI_16b_inplace_refresh_array(f, 1 << m_f);
  MSK_uint16_t Q[2 * (1 << (PARAM_FFT - 2)) + 1] = {0};
  MSK_uint16_t P[2 * (1 << (PARAM_FFT - 2)) + 1] = {0};

  MSK_uint16_t Q0[1 << (PARAM_FFT - 2)] = {0};
  MSK_uint16_t Q1[1 << (PARAM_FFT - 2)] = {0};
  MSK_uint16_t P0[1 << (PARAM_FFT - 2)] = {0};
  MSK_uint16_t P1[1 << (PARAM_FFT - 2)] = {0};

  size_t i, n;

  n = 1;
  n <<= (m_f - 2);
  // No need to move the cursors, just to multiply the amount of data to copy
  MSK_16b_to_16b_copy_array(Q, f + 3 * n, n);
  MSK_16b_to_16b_copy_array(Q + n, f + 3 * n, n);
  MSK_16b_to_16b_copy_array(P, f, 2 * n);

  MSK_SNI_16b_inplace_refresh_array(Q, 2 * (1 << (PARAM_FFT - 2)) + 1);
  for (i = 0; i < n; i++) {
    MSK_16b_xor(Q[i], Q[i], f[2 * n + i]);
    MSK_16b_xor(P[n + i], P[n + i], Q[i]);
  }

  MSK_radix(Q0, Q1, Q, m_f - 1);
  MSK_radix(P0, P1, P, m_f - 1);

  MSK_16b_to_16b_copy_array(f0, P0, n);
  MSK_16b_to_16b_copy_array(f0 + n, Q0, n);
  MSK_16b_to_16b_copy_array(f1, P1, n);
  MSK_16b_to_16b_copy_array(f1 + n, Q1, n);
}

/**
 * @brief Evaluate f at all subset sums of given set
 *
 * Subroutine of function PQCLEAN_HQC128_CLEAN_fft.
 *
 * @param[out] [w] Array of 16-bits N_SHARES-shares boolean-masked values
 * @param[inout] [f] Array of 16-bits N_SHARES-shares boolean-masked values
 * @param[in] f_coeffs Number of coefficients of f
 * @param[in] m Number of betas
 * @param[in] m_f Rounded-up log2 of number of coefficients of f (one more than
 * degree)
 * @param[in] betas FFT constants
 */
void MSK_fft_rec(MSK_uint16_t w[], MSK_uint16_t f[], const size_t f_coeffs,
                 const uint8_t m, const uint32_t m_f,
                 const uint16_t *const betas) {
  MSK_uint16_t f0[1 << (PARAM_FFT - 2)] = {0};
  MSK_uint16_t f1[1 << (PARAM_FFT - 2)] = {0};
  uint16_t gammas[PARAM_M - 2] = {0};
  uint16_t deltas[PARAM_M - 2] = {0};
  uint16_t gammas_sums[1 << (PARAM_M - 2)] = {0};
  MSK_uint16_t u[1 << (PARAM_M - 2)] = {0};
  MSK_uint16_t v[1 << (PARAM_M - 2)] = {0};
  MSK_uint16_t tmp[PARAM_M - (PARAM_FFT - 1)] = {0};

  uint16_t beta_m_pow;
  size_t i, j, k;
  size_t x;

  // Step 1
  if (m_f == 1) {
    for (i = 0; i < m; i++) {
      for (uint8_t d = 0; d < N_SHARES; d++) {
        tmp[i][d] = gf_mul(betas[i], f[1][d]);
      }
    }

    // w[0] = f[0];
    MSK_16b_to_16b_copy(w[0], f[0]);
    x = 1;
    for (j = 0; j < m; j++) {
      for (k = 0; k < x; k++) {
        MSK_SNI_16b_inplace_refresh(w[k]);
        MSK_16b_xor(w[x + k], w[k], tmp[j]);
      }
      x <<= 1;
    }

    return;
  }

  // Step 2: compute g
  if (betas[m - 1] != 1) {
    beta_m_pow = 1;
    x = 1;
    x <<= m_f;
    for (i = 1; i < x; i++) {
      beta_m_pow = gf_mul(beta_m_pow, betas[m - 1]);
      for (uint8_t d = 0; d < N_SHARES; d++) {
        f[i][d] = gf_mul(beta_m_pow, f[i][d]);
      }
    }
  }

  // Step 3
  MSK_radix(f0, f1, f, m_f);

  // Step 4: compute gammas and deltas
  for (i = 0; i + 1 < m; i++) {
    gammas[i] = gf_mul(betas[i], gf_inverse(betas[m - 1]));
    deltas[i] = gf_square(gammas[i]) ^ gammas[i];
  }

  // Compute gammas sums
  compute_subset_sums(gammas_sums, gammas, m - 1);

  // Step 5
  MSK_fft_rec(u, f0, (f_coeffs + 1) / 2, m - 1, m_f - 1, deltas);

  k = 1;
  k <<= ((m - 1) & 0xf); // let compiler know that m-1 is small.
  if (f_coeffs <= 3) {   // 3-coefficient polynomial f case: f1 is constant
    // w[0] = u[0];
    MSK_16b_to_16b_copy(w[0], u[0]);
    // w[k] = u[0] ^ f1[0];
    MSK_SNI_16b_inplace_refresh(u[0]);
    MSK_16b_xor(w[k], u[0], f1[0]);

    for (i = 1; i < k; i++) {
      MSK_SNI_16b_inplace_refresh(u[i]);
      for (uint8_t d = 0; d < N_SHARES; d++) {
        w[i][d] = u[i][d] ^ gf_mul(gammas_sums[i], f1[0][d]);
      }
      // w[k + i][d] = w[i][d] ^ f1[0][d];
      MSK_SNI_16b_inplace_refresh(f1[0]);
      MSK_16b_xor(w[k + i], w[i], f1[0]);
    }
  } else {
    MSK_fft_rec(v, f1, f_coeffs / 2, m - 1, m_f - 1, deltas);

    // Step 6
    MSK_16b_to_16b_copy_array(w + k, v, k);
    MSK_16b_to_16b_copy(w[0], u[0]);

    // w[k][d] ^= u[0][d];
    MSK_SNI_16b_inplace_refresh(u[0]);
    MSK_16b_xor(w[k], w[k], u[0]);

    for (i = 1; i < k; i++) {
      MSK_SNI_16b_inplace_refresh(u[i]);
      for (uint8_t d = 0; d < N_SHARES; d++) {
        w[i][d] = u[i][d] ^ gf_mul(gammas_sums[i], v[i][d]);
      }
      // w[k + i][d] ^= w[i][d];
      MSK_SNI_16b_inplace_refresh(w[i]);
      MSK_16b_xor(w[k + i], w[k + i], w[i]);
    }
  }
}

void MSK_fft(MSK_uint16_t w[], MSK_uint16_t f[], const size_t f_coeffs) {
  uint16_t betas[PARAM_M - 1] = {0};
  uint16_t betas_sums[1 << (PARAM_M - 1)] = {0};
  MSK_uint16_t f0[1 << (PARAM_FFT - 1)] = {0};
  MSK_uint16_t f1[1 << (PARAM_FFT - 1)] = {0};
  uint16_t deltas[PARAM_M - 1] = {0};
  MSK_uint16_t u[1 << (PARAM_M - 1)] = {0};
  MSK_uint16_t v[1 << (PARAM_M - 1)] = {0};

  size_t i, k;

  // Follows Gao and Mateer algorithm
  compute_fft_betas(betas);

  // Step 1: PARAM_FFT > 1, nothing to do

  // Compute gammas sums
  compute_subset_sums(betas_sums, betas, PARAM_M - 1);

  // Step 2: beta_m = 1, nothing to do

  // Step 3
  MSK_radix(f0, f1, f, PARAM_FFT);

  // Step 4: Compute deltas
  for (i = 0; i < PARAM_M - 1; i++) {
    deltas[i] = gf_square(betas[i]) ^ betas[i]; // no need to mask
  }

  // Step 5
  MSK_fft_rec(u, f0, (f_coeffs + 1) / 2, PARAM_M - 1, PARAM_FFT - 1, deltas);
  MSK_fft_rec(v, f1, f_coeffs / 2, PARAM_M - 1, PARAM_FFT - 1, deltas);

  k = 1 << (PARAM_M - 1);
  // Step 6, 7 and error polynomial computation
  MSK_16b_to_16b_copy_array(w + k, v, k);

  // Check if 0 is root
  // w[0] = u[0];
  MSK_16b_to_16b_copy(w[0], u[0]);

  // Check if 1 is root
  // w[k] ^= u[0]
  MSK_SNI_16b_inplace_refresh(u[0]);
  MSK_16b_xor(w[k], w[k], u[0]);

  for (i = 1; i < k; i++) {
    MSK_SNI_16b_inplace_refresh(u[i]);
    for (uint8_t d = 0; d < N_SHARES; d++) {
      w[i][d] = u[i][d] ^ gf_mul(betas_sums[i], v[i][d]);
    }
    // w[k + i][d] ^= w[i][d];
    MSK_16b_xor(w[k + i], w[k + i], w[i]);
  }
}

void MSK_fft_retrieve_error_poly(MSK_uint8_t error[1 << PARAM_M],
                                 MSK_uint16_t w[1 << PARAM_M]) {
  uint16_t gammas[PARAM_M - 1] = {0};
  uint16_t gammas_sums[1 << (PARAM_M - 1)] = {0};
  uint16_t k = 1 << (PARAM_M - 1);
  size_t i, index1, index2;
  MSK_uint16_t wso[2 * k];

  compute_fft_betas(gammas);
  compute_subset_sums(gammas_sums, gammas, PARAM_M - 1);

  // no path from input to output
  MSK_SNI_16b_inplace_refresh_array(w, 1 << PARAM_M);

  // we precompute all (-w)>>15;
  for (i = 0; i < 2 * k; i++) { // - w
    MSK_16b_to_16b_copy(wso[i], w[i]);
    MSK_16b_neg(wso[i], wso[i]);
    MSK_16b_right_shift(wso[i], wso[i], 15);
  }

  // error[0] ^= ((-w[0]) >> 15) ^ ((-w[k]) >> 15);
  for (uint8_t d = 0; d < N_SHARES; d++) {
    error[0][d] ^= wso[0][d] ^ wso[k][d];
  }

  for (i = 1; i < k; i++) {
    index1 = PARAM_GF_MUL_ORDER - gf_log[gammas_sums[i]];
    index2 = PARAM_GF_MUL_ORDER - gf_log[gammas_sums[i] ^ 1];

    // error[index1] ^= 1 ^ ((-w[i]) >> 15);
    // error[index2] ^= 1 ^ ((-w[i+k]) >> 15);
    error[index1][0] ^= 1;
    error[index2][0] ^= 1;
    for (uint8_t d = 0; d < N_SHARES; d++) {
      error[index1][d] ^= wso[i][d];
      error[index2][d] ^= wso[i + k][d];
    }
  }
}
