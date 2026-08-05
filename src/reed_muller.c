/**
 * @file reed_muller.c
 * @brief Constant time implementation of Reed-Muller code RM(1,7)
 * with masked decoder
 */

#include <string.h>

#include "reed_muller.h"

// number of repeated code words
#define MULTIPLICITY CEIL_DIVIDE(PARAM_N2, 128)

// copy bit 0 into all bits of a 32 bit value
#define BIT0MASK(x) (uint32_t)(-((x) & 1))

/**
 * @brief Masked copy bit 0 of x to all bits of 32-bits out
 *
 * @param[in] [x,] input N_SHARES-shares boolean-masked byte
 * @param[out] [out,] output 32-bits N_SHARES-shares boolean-masked value
 */
void MSK_bit0mask(MSK_uint32_t out, const MSK_uint8_t x) {
  memset(out, 0, 4 * (N_SHARES));
  for (size_t d = 0; d < N_SHARES; d++) {
    out[d] = x[d] & 1;
  }
  MSK_32b_neg(out, out);
}

/**
 * @brief Encode a single byte into a single codeword using RM(1,7)
 *
 * Encoding matrix of this code:
 * bit pattern (note that bits are numbered big endian)
 * 0   aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
 * 1   cccccccc cccccccc cccccccc cccccccc
 * 2   f0f0f0f0 f0f0f0f0 f0f0f0f0 f0f0f0f0
 * 3   ff00ff00 ff00ff00 ff00ff00 ff00ff00
 * 4   ffff0000 ffff0000 ffff0000 ffff0000
 * 5   ffffffff 00000000 ffffffff 00000000
 * 6   ffffffff ffffffff 00000000 00000000
 * 7   ffffffff ffffffff ffffffff ffffffff
 *
 * @param[out] cword RM(1,7) result codeword, array of two 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] [message] Input message, N_SHARES-shares boolean-masked byte
 */
void MSK_encode(MSK_uint64_t cword[], MSK_uint8_t message) {
  MSK_uint32_t first_word;
  MSK_uint32_t tmp_w1, tmp_w2, tmp_w3, tmp_w4, tmp_w5, tmp_w6, tmp_w7;
  MSK_uint64_t tmp_w64 = {0};
  MSK_uint8_t ms1, ms2, ms3, ms4, ms5, ms6, ms7;

  MSK_SNI_8b_inplace_refresh(message);

  MSK_8b_right_shift(ms1, message, 1);
  MSK_8b_right_shift(ms2, message, 2);
  MSK_8b_right_shift(ms3, message, 3);
  MSK_8b_right_shift(ms4, message, 4);
  MSK_8b_right_shift(ms5, message, 5);
  MSK_8b_right_shift(ms6, message, 6);
  MSK_8b_right_shift(ms7, message, 7);

  // bit 7 flips all the bits, do that first to save work
  // first_word = BIT0MASK(message >> 7);
  MSK_bit0mask(first_word, ms7);
#ifdef MIMOSNI
  MSK_SNI_32b_inplace_refresh(first_word);
#endif

  // bits 0, 1, 2, 3, 4 are the same for all four longs
  // (Warning: in the bit matrix above, low bits are at the left!)
  // first_word ^= BIT0MASK(message >> 0) & 0xaaaaaaaa;
  MSK_SNI_8b_inplace_refresh(message);
  MSK_bit0mask(tmp_w7, message);
  for (size_t d = 0; d < N_SHARES; d++) {
    first_word[d] ^= tmp_w7[d] & 0xaaaaaaaa;
  }

  // first_word ^= BIT0MASK(message >> 1) & 0xcccccccc;
  MSK_bit0mask(tmp_w1, ms1);
  for (size_t d = 0; d < N_SHARES; d++) {
    first_word[d] ^= tmp_w1[d] & 0xcccccccc;
  }
  // first_word ^= BIT0MASK(message >> 2) & 0xf0f0f0f0;
  MSK_bit0mask(tmp_w2, ms2);
  for (size_t d = 0; d < N_SHARES; d++) {
    first_word[d] ^= tmp_w2[d] & 0xf0f0f0f0;
  }
  // first_word ^= BIT0MASK(message >> 3) & 0xff00ff00;
  MSK_bit0mask(tmp_w3, ms3);
  for (size_t d = 0; d < N_SHARES; d++) {
    first_word[d] ^= tmp_w3[d] & 0xff00ff00;
  }
  // first_word ^= BIT0MASK(message >> 4) & 0xffff0000;
  MSK_bit0mask(tmp_w4, ms4);
  for (size_t d = 0; d < N_SHARES; d++) {
    first_word[d] ^= tmp_w4[d] & 0xffff0000;
  }
  // we can store this in the first quarter
  MSK_32b_to_64b_copy(cword[0], first_word);

  // bit 5 flips entries 1 and 3; bit 6 flips 2 and 3
  // first_word ^= BIT0MASK(message >> 5);
  MSK_bit0mask(tmp_w5, ms5);
  MSK_32b_xor(first_word, first_word, tmp_w5);
  MSK_32b_to_64b_copy(tmp_w64, first_word);
  MSK_64b_left_shift(tmp_w64, tmp_w64, 32);

#ifdef MIMOSNI
  MSK_SNI_64b_inplace_refresh(cword[0]);
  MSK_SNI_64b_inplace_refresh(tmp_w64);
#endif
  // cword[0] |= (uint64_t)first_word << 32;
  // MSK_SNI_64b_or(cword[0],cword[0],tmp_w64);
  MSK_64b_xor(cword[0], cword[0], tmp_w64);

  // first_word ^= BIT0MASK(message >> 6);
  MSK_bit0mask(tmp_w6, ms6);
  MSK_32b_xor(first_word, first_word, tmp_w6);
  MSK_32b_to_64b_copy(cword[1], first_word);
  MSK_64b_left_shift(cword[1], cword[1], 32);
  MSK_SNI_64b_inplace_refresh(cword[1]);

  // first_word ^= BIT0MASK(message >> 5);
  MSK_SNI_32b_inplace_refresh(tmp_w5);
  MSK_32b_xor(first_word, first_word, tmp_w5);
  MSK_32b_to_64b_copy(tmp_w64, first_word);

  // MSK_SNI_64b_or(cword[1],cword[1],tmp_w64);
  // cword[1] |= first_word;
  MSK_64b_xor(cword[1], cword[1], tmp_w64);
}

/**
 * @brief Hadamard transform
 *
 * Perform hadamard transform of src and store result in dst
 * src is overwritten
 *
 * @param[out] [dst] Transform result, array of 128 16-bits N_SHARES-shares
 * boolean-masked values
 * @param[inout] src Input expanded codeword, array of 128 16-bits
 * N_SHARES-shares boolean-masked values (refreshed and overwritten)
 */
void MSK_hadamard(MSK_uint16_t dst[128], MSK_uint16_t src[128]) {
  // the passes move data:
  // src -> dst -> src -> dst -> src -> dst -> src -> dst
  // using p1 and p2 alternately
  MSK_uint16_t(*p1) = src;
  MSK_uint16_t(*p2) = dst;
  MSK_uint16_t(*p3);

  for (size_t pass = 0; pass < 7; ++pass) {
    for (size_t i = 0; i < 64; ++i) {
      // p2[i] = p1[2 * i] + p1[2 * i + 1];
      // p2[i + 64] = p1[2 * i] - p1[2 * i + 1];
      MSK_SNI_16b_inplace_refresh(p1[2 * i]);
      MSK_16b_add(p2[i], p1[2 * i], p1[2 * i + 1]);
      // MSK_16b_refresh(p2[i]);
      MSK_SNI_16b_inplace_refresh(p1[2 * i]);
      MSK_16b_sub(p2[i + 64], p1[2 * i], p1[2 * i + 1]);
    }
    // swap p1, p2 for next round
    p3 = p1;
    p1 = p2;
    p2 = p3;
  }
}

/**
 * @brief Add multiple codewords into expanded codeword
 *
 * Accesses memory in order
 * Note: this does not write the codewords as -1 or +1 as the green machine does
 * instead, just 0 and 1 is used.
 * The resulting hadamard transform has:
 * all values are halved
 * the first entry is 64 too high
 *
 * @param[out] [dest] Expanded codeword, array of 128 16-bits N_SHARES-shares
 * boolean-masked values
 * @param[inout] src Input codeword, array of 2 * MULTIPLICITY 64-bits
 * N_SHARES-shares boolean-masked values (refreshed)
 */
void MSK_expand_and_sum(MSK_uint16_t dest[128],
                        MSK_uint64_t src[2 * MULTIPLICITY]) {
  // start with the first copy
  for (size_t part = 0; part < 2; ++part) {
    for (size_t bit = 0; bit < 64; ++bit) {
      for (size_t d = 0; d < N_SHARES; d++) {
        dest[part * 64 + bit][d] = ((src[part][d] >> bit) & 1);
      }
      MSK_SNI_16b_inplace_refresh(dest[part * 64 + bit]);
    }
  }

  // sum the rest of the copies
  for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
    for (size_t part = 0; part < 2; ++part) {
      for (size_t bit = 0; bit < 64; ++bit) {
        // dest[part * 64 + bit] += (uint16_t) ((src[2 * copy + part] >> bit) &
        // 1);
        MSK_uint16_t tmp_src;
        MSK_SNI_64b_inplace_refresh(src[2 * copy + part]);
        for (size_t d = 0; d < N_SHARES; d++) {
          tmp_src[d] = (uint16_t)((src[2 * copy + part][d] >> bit) & 1);
        }
        MSK_16b_add(dest[part * 64 + bit], dest[part * 64 + bit], tmp_src);
      }
    }
  }
}

void MSK_reed_muller_encode(MSK_uint64_t cdw[VEC_N1N2_SIZE_64],
                            MSK_uint8_t msg[VEC_N1_SIZE_BYTES]) {
  for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
    // encode first word
    MSK_encode(&cdw[2 * i * MULTIPLICITY], msg[i]);
    // copy to other identical codewords
    for (size_t copy = 1; copy < MULTIPLICITY; ++copy) {
      MSK_64b_to_64b_copy_array(cdw + 2 * i * MULTIPLICITY + 2 * copy,
                                cdw + 2 * i * MULTIPLICITY, 2);
    } // refresh either input or output
  }
}

/**
 * @brief Finding the location of the highest value
 *
 * This is the final step of the green machine: find the location of the highest
 * value, and add 128 if the peak is positive. If there are two identical peaks,
 * the peak with smallest value in the lowest 7 bits it taken
 *
 * @param[out] [res] Decoded N_SHARES-shares boolean-masked byte
 * @param[in] transform Transformed expanded codeword, array of 128 16-bits
 * N_SHARES-shares boolean-masked values
 */
void MSK_find_peaks(MSK_uint8_t res, MSK_uint16_t transform[128]) {
  MSK_uint16_t peak_abs = {0};
  MSK_uint16_t peak = {0};
  MSK_uint16_t pos = {0};
  MSK_uint16_t abs, mask;
  MSK_uint16_t t;
  uint8_t i;
  MSK_uint16_t t15, mt15, minus_t, txmt, tmpabs;
  MSK_uint16_t pa;
  MSK_uint16_t pt, pi, paa;
  MSK_uint16_t mpt, mpi, mpaa;

  // For every 16 bits value t in transform
  for (i = 0; i < 128; ++i) {
    MSK_16b_to_16b_copy(t, transform[i]);

    // Step 1
    // abs = t ^ ((uint16_t)(-(t >> 15)) & (t ^ -t));
    MSK_16b_neg(minus_t, t);
    MSK_SNI_16b_inplace_refresh(t);
    // t15[d] = t[d]>>15;
    MSK_16b_right_shift(t15, t, 15);
    MSK_SNI_16b_inplace_refresh(t);
    MSK_16b_xor(txmt, t, minus_t);
    MSK_16b_neg(mt15, t15);
#ifdef MIMOSNI
    MSK_SNI_16b_inplace_refresh(txmt);
#endif
    MSK_SNI_16b_and(tmpabs, mt15, txmt);
    MSK_SNI_16b_inplace_refresh(t);
    MSK_16b_xor(abs, tmpabs, t);

// Step 2
// mask = -(((uint16_t)(peak_abs - abs)) >> 15);
#ifdef MIMOSNI
    MSK_SNI_16b_inplace_refresh(peak_abs);
#endif
    MSK_16b_sub(pa, peak_abs, abs);
    MSK_16b_right_shift(pa, pa, 15);
    MSK_16b_neg(mask, pa);

    // Step 3
    // peak ^= mask & (peak ^ t);
    // pos ^= mask & (pos ^ i);
    // peak_abs ^= mask & (peak_abs ^ abs);
    MSK_SNI_16b_inplace_refresh(t);
    MSK_SNI_16b_inplace_refresh(peak_abs);
    MSK_16b_xor(pt, peak, t);
    MSK_16b_xor(paa, peak_abs, abs);
    MSK_16b_to_16b_copy(pi, pos);
    pi[0] ^= i; //*4+j;

#ifdef MIMOSNI
    MSK_SNI_16b_inplace_refresh(mask);
#endif
    MSK_SNI_16b_and(mpt, mask, pt);
    MSK_SNI_16b_inplace_refresh(mask);
    MSK_SNI_16b_and(mpi, mask, pi);
    MSK_SNI_16b_inplace_refresh(mask);
    MSK_SNI_16b_and(mpaa, mask, paa);

    MSK_SNI_16b_inplace_refresh(mpt);
    MSK_SNI_16b_inplace_refresh(mpi);
    MSK_SNI_16b_inplace_refresh(mpaa);
    MSK_16b_xor(peak, peak, mpt);
    MSK_16b_xor(pos, pos, mpi);
    MSK_16b_xor(peak_abs, peak_abs, mpaa);
  }

  // Final step : set bit 7
  // pos |= 128 & (uint16_t)((peak >> 15) - 1);
  MSK_uint16_t tmpeak, tmp;

  MSK_16b_right_shift(tmpeak, peak, 15);
  MSK_16b_semi_add(tmpeak, tmpeak, UINT16_MAX); // - 1
  MSK_16b_semi_and(tmp, tmpeak, 128);

  // a OR b = a XOR b XOR (a AND b)
  MSK_uint16_t pat;
#ifdef MIMOSNI
  MSK_SNI_16b_inplace_refresh(pos);
#endif
  MSK_SNI_16b_and(pat, pos, tmp);

  MSK_SNI_16b_inplace_refresh(pos);
  MSK_SNI_16b_inplace_refresh(tmp);

  MSK_16b_xor3(tmp, pos, tmp, pat);
  MSK_16b_to_8b_copy(res, tmp);
}

void MSK_reed_muller_decode(MSK_uint8_t msg[VEC_N1_SIZE_BYTES],
                            MSK_uint64_t cdw[VEC_N1N2_SIZE_64]) {
  MSK_uint16_t expanded[128];
  MSK_uint16_t transform[128];
  MSK_uint16_t mul64 = {0};

  for (size_t i = 0; i < VEC_N1_SIZE_BYTES; ++i) {
    // collect the codewords
    MSK_expand_and_sum(
        expanded, &cdw[2 * i * MULTIPLICITY]); // MULTIPLICITY : 3 for HQC128

    // apply hadamard transform
    MSK_hadamard(transform, expanded);

    // fix the first entry to get the half Hadamard transform
    MSK_16b_mask(mul64, 64 * MULTIPLICITY);
    MSK_16b_sub(transform[0], transform[0], mul64);

    // finish the decoding
    MSK_find_peaks(msg[i], transform);
  }
}
