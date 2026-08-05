/**
 * @file barrett.h
 * @brief Masked Barrett reduction
 */

#ifndef MSK_BARRETT_H
#define MSK_BARRETT_H

#include "mask.h"

/**
 * @brief Masked Barrett reduction
 *
 * Compute remainder of integer division using a constant time masked Barrett
 * reduction. Note: boolean-arithmetic masking conversions are used to speedup
 * multiplications.
 *
 * @param[out] [x] Result (x = a % (N_PARAM - i)), N_SHARES-shares 32-bits
 * boolean-masked value
 * @param[inout] [a] Operand, N_SHARES-shares 32-bits boolean-masked value
 * (refreshed)
 * @param[in] i Modulus offset, 8-bits value
 */
void MSK_barrett(MSK_uint32_t x, MSK_uint32_t a, const uint8_t i);

#endif
