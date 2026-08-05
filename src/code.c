/**
 * @file code.c
 * @brief Implementation of concatenated code
 */

#include "code.h"
#include "reed_muller.h"
#include "reed_solomon.h"

void MSK_code_encode(MSK_uint64_t em[VEC_N1N2_SIZE_64],
                     MSK_uint8_t m[VEC_K_SIZE_BYTES]) {
  MSK_uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};

  MSK_rs_encode(tmp, m);
  MSK_reed_muller_encode(em, tmp);

#if VERBOSITY_LEVEL != 0
  MSK_8b_fprintf_array(stdout, "\n\nReed-Solomon code word: ", tmp,
                       VEC_N1_SIZE_BYTES, "\n");
  MSK_64b_fprintf_array(stdout, "\nConcatenated code word: ", em,
                        VEC_N1N2_SIZE_64, "\n");
#endif
}

void MSK_code_decode(MSK_uint8_t m[VEC_K_SIZE_BYTES],
                     MSK_uint64_t em[VEC_N1N2_SIZE_64]) {
  MSK_uint8_t tmp[VEC_N1_SIZE_BYTES] = {0};

  MSK_reed_muller_decode(tmp, em);
  MSK_reed_solomon_decode(m, tmp);

#if VERBOSITY_LEVEL != 0
  MSK_8b_fprintf_array(stdout,
                       "\n\nReed-Muller decoding result (the input for the "
                       "Reed-Solomon decoding algorithm): \n",
                       tmp, VEC_N1_SIZE_BYTES, "\n\n");
#endif
}
