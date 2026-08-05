/**
 * @file reed_muller.h
 * @brief Constant time Reed-Muller code RM(1,7) with masked decoder
 */

#ifndef MSK_REED_MULLER_H
#define MSK_REED_MULLER_H

#include "mask.h"
#include "parameters.h"

/**
 * @brief Decodes the received word
 *
 * Decoding uses fast hadamard transform, for a more complete picture on
 * Reed-Muller decoding, see MacWilliams, Florence Jessie, and Neil James
 * Alexander Sloane. The theory of error-correcting codes codes @cite
 * macwilliams1977theory
 *
 * @param[out] [msg] Decoded message, array of VEC_N1_SIZE_64 N_SHARES-shares
 * 64-bits boolean-masked values
 * @param[inout] [cdw] Received code word, array of VEC_N1N2_SIZE_64 64-bits
 * N_SHARES-shares boolean-masked values (refreshed)
 */
void MSK_reed_muller_decode(MSK_uint8_t msg[VEC_N1_SIZE_BYTES],
                            MSK_uint64_t cdw[VEC_N1N2_SIZE_64]);

/**
 * @brief Encodes the received word
 *
 * @param[out] [cdw] Received code word, array of VEC_N1N2_SIZE_64 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] [msg] Message to encode, array of VEC_N1_SIZE_64 64-bits
 * N_SHARES-shares boolean-masked values (refreshed)
 */
void MSK_reed_muller_encode(MSK_uint64_t cdw[VEC_N1N2_SIZE_64],
                            MSK_uint8_t msg[VEC_N1_SIZE_BYTES]);

#endif
