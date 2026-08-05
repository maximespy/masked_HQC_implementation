/**
 * @file code.h
 * @brief Reed-Solomon - Reed-Muller concatenated code
 */

#ifndef MSK_CODE_H
#define MSK_CODE_H

#include "mask.h"
#include "parameters.h"

/**
 * @brief Encode message m to codeword em using concatenated code
 *
 * First encode message with Reed-Solomon code, then with duplicated
 * Reed-Muller code
 *
 * @param[out] [em] Tensor codeword, array of VEC_N1N2_SIZE_64 N_SHARES-shares
 * 64-bits boolean-masked values
 * @param[inout] [m] Message to encode, array of VEC_K_SIZE_BYTES
 * N_SHARES-shares boolean-masked bytes (refreshed)
 */
void MSK_code_encode(MSK_uint64_t em[VEC_N1N2_SIZE_64],
                     MSK_uint8_t m[VEC_K_SIZE_BYTES]);

/**
 * @brief Decode codeword em to message m using concatenated code
 *
 * @param[out] [m] Message to encode, array of VEC_K_SIZE_BYTES
 * N_SHARES-shares boolean-masked bytes
 * @param[inout] [em] Tensor codeword, array of VEC_N1N2_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values (refreshed)
 */
void MSK_code_decode(MSK_uint8_t m[VEC_K_SIZE_BYTES],
                     MSK_uint64_t em[VEC_N1N2_SIZE_64]);

#endif
