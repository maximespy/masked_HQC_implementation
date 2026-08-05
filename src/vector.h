/**
 * @file vector.h
 * @brief Vector operations: sampling, additions, resize, compare
 */

#ifndef MSK_VECTOR_H
#define MSK_VECTOR_H

#include <inttypes.h>

#include "mask.h"
#include "parameters.h"
#include "shake_prng.h"

/**
 * @brief Generates a masked vector with given Hamming weight using Barrett

 * @param[out] [v] Random vector, an array of VEC_N_SIZE_64 64-bits
 N_SHARES-shares boolean-masked values
 * @param[inout] [ctx] Pointer to the masked context of the seed expander
 * @param[in] weight Hamming weight of random vector to generate
 */
void MSK_vect_set_random_fixed_weight(MSK_uint64_t v[],
                                      MSK_seedexpander_ctx *ctx,
                                      const size_t weight);

/**
 * @brief Masked addition of two vectors
 *
 * @param[out] [o] Result vector, an array of size 64-bits N_SHARES-shares
 * boolean-masked values
 * @param[inout] [v1] First vector operand, an array of size 64-bits
 * N_SHARES-shares boolean-masked values (refreshed)
 * @param[inout] [v2] First vector operand, an array of size 64-bits
 * N_SHARES-shares boolean-masked values (refreshed)
 * @param[in] size Size of vectors
 */
void MSK_vect_add(MSK_uint64_t o[], MSK_uint64_t v1[], MSK_uint64_t v2[],
                  const size_t size);

/**
 * @brief Generate a random binary vector of dimension PARAM_N
 *
 * Use HQC seed expander, drop the extra bits using a mask.
 *
 * @param[out] v Random binary vector, an array of VEC_N_SIZE_64 64-bits values
 * @param[inout] ctx Pointer to the context of the seed expander
 */
void vect_set_random(uint64_t v[VEC_N_SIZE_64], seedexpander_ctx *ctx);

/**
 * @brief Masked vector comparison
 *
 * @param[inout] [r] Comparison result, a 64-bits N_SHARES-shares boolean-masked
 * value. If v1==v2 r is unmodified, else it is or-ed with 1
 * @param[inout] [v1] First vector operand, an array of size 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[inout] [v2] Second vector operand, an array of size 64-bits
 * N_SHARES-shares boolean-masked values
 * @param[in] size Size of vectors
 */
void MSK_vect_compare(MSK_uint64_t r, MSK_uint64_t v1[], MSK_uint64_t v2[],
                      const size_t size);

/**
 * @brief Resize a vector so that it contains size_o bits
 *
 * @param[out] [o] Output vector, an array of size_o/64 64-bits N_SHARES-shares
 * boolean masked values. The most significant bits of the last value are
 * zeroed if size_o is not a multiple of 64.
 * @param[in] size_o Bit-size of output vector
 * @param[in] [v] Input vector, an array of size_v/64 64-bits N_SHARES-shares
 * boolean masked values.
 * @param[in] size_v Bit-size of input vector
 */
void MSK_vect_resize(MSK_uint64_t o[], const uint32_t size_o,
                     const MSK_uint64_t v[], const uint32_t size_v);

#endif
