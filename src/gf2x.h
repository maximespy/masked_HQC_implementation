/**
 * @file gf2x.h
 * @brief \f$GF_2\f$ polynomials modulo \f$ X^n - 1\f$
 */

#ifndef MSK_GF2X_H
#define MSK_GF2X_H

#include "mask.h"
#include "parameters.h"

/**
 * @brief Multiply two polynomials modulo \f$X^n - 1\f$.
 *
 * @param[out] [o] \f$v_1 \times v_2\f$ product, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[inout] [v1] First polynomial operand, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values (refreshed)
 * @param[inout] [v2] Second polynomial operand, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values (refreshed)
 */
void MSK_vect_mul(MSK_uint64_t o[VEC_N_SIZE_64], MSK_uint64_t v1[VEC_N_SIZE_64],
                  MSK_uint64_t v2[VEC_N_SIZE_64]);

#endif
