/**
 * @file gf.h
 * @brief Computations in Galois field \f$GF_2^8\f$ (GF for short) with
 * primitive polynomial \f$1 + x^2 + x^3 + x^4 + x^8\f$
 */

#ifndef MSK_GF_H
#define MSK_GF_H

#include <stdint.h>

#include "mask.h"

//------------------------ constants--------------------------

/**
 * @brief Powers of the root alpha of \f$1 + x^2 + x^3 + x^4 + x^8\f$
 *
 * The last two elements are needed by the PQCLEAN_HQC128_CLEAN_gf_mul function
 * (for example if both elements to multiply are zero).
 */
extern const uint16_t gf_exp[258];

/**
 * @brief Logarithm of elements of GF to the base alpha (root of \f$1 + x^2 +
 * x^3 + x^4 + x^8\f$). The logarithm of 0 is set to 0 by convention.
 */
extern const uint16_t gf_log[256];

//------------------------ masked operations

/**
 * @brief Inverse boolean-masked element of GF
 *
 * Note: use the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 *
 * @param[out] [x] N_SHARES-shares 16-bits boolean-masked inverse of a if a !=
 * 0 or 0 if a = 0
 * @param[inout] [a] N_SHARES-shares 16-bits boolean-masked element of GF
 * (refreshed)
 */
void MSK_gf_inverse(MSK_uint16_t x, MSK_uint16_t a);

/**
 * @brief Multiply two boolean-masked elements of GF
 *
 * @param[out] [x] N_SHARES-shares 16-bits boolean-masked product a*b
 * @param[in] [a] N_SHARES-shares 16-bits boolean-masked first operand
 * @param[in] [b] N_SHARES-shares 16-bits boolean-masked second operand
 */
void MSK_gf_mul(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Multiply a boolean-masked element of GF with an unmasked one
 *
 * @param[out] [x] N_SHARES-shares 16-bits boolean-masked product a*b
 * @param[inout] [a] N_SHARES-shares 16-bits boolean-masked first operand
 * (refreshed)
 * @param[in] b Unmasked second operand
 */
void MSK_semi_gf_mul(MSK_uint16_t x, MSK_uint16_t a, const uint16_t b);

/**
 * @brief Multiply a boolean-masked element of GF with an unmasked one
 *
 * Note: 8-bits version of MSK_semi_gf_mul
 *
 * @param[out] [x] N_SHARES-shares 8-bits boolean-masked product a*b
 * @param[inout] [a] N_SHARES-shares 8-bits boolean-masked first operand
 * (refreshed)
 * @param[in] b Unmasked second operand
 */
void MSK_8b_semi_gf_mul(MSK_uint8_t x, MSK_uint8_t a, const uint8_t b);

//------------------------ unmasked operations

/**
 * @brief Reduce polynomial a modulo primitive polynomial GF_POLY.
 *
 * @return Reduced polynomial (a mod GF_POLY)
 * @param[inout] a Polynomial of degree less than 64
 * @param[in] deg Degree of polynomial a
 */
uint16_t gf_reduce(uint64_t a, const uint8_t deg);

/**
 * @brief Multiply two elements of GF
 *
 * @return Product a*b
 * @param[in] a First operand
 * @param[in] b Second operand
 */
uint16_t gf_mul(const uint16_t a, const uint16_t b);

/**
 * @brief Square an element of GF
 *
 * @return \f$a^2\f$
 * @param[in] a Element of GF
 */
uint16_t gf_square(const uint16_t a);

/**
 * @brief Inverse element of GF
 *
 * Note: uses the addition chain 1 2 3 4 7 11 15 30 60 120 127 254
 *
 * @return Inverse of a if a != 0 or 0 if a = 0
 * @param[inout] a Element of GF
 */
uint16_t gf_inverse(const uint16_t a);

#endif
