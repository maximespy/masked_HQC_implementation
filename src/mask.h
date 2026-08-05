/**
 * @file mask.h
 * @brief Base operations on masked values
 */

#ifndef MSK_MASK_H
#define MSK_MASK_H

#include <inttypes.h>
#include <stdio.h>

/**********************************************************************
 * Masking order and number of shares per sensitive value          *
 **********************************************************************/

/**
 * @brief Masking order (number of shares minus 1 of masked values)
 */
#ifndef MASKING_ORDER
#define MASKING_ORDER 1
#endif

/**
 * @brief Number of shares of masked values
 */
#define N_SHARES (MASKING_ORDER + 1)

/**********************************************************************
 * Basic types for masked values                                   *
 **********************************************************************/

/**
 * @brief 8-bits N_SHARES-shares masked values
 */
typedef uint8_t MSK_uint8_t[N_SHARES];

/**
 * @brief 16-bits N_SHARES-shares masked values
 */
typedef uint16_t MSK_uint16_t[N_SHARES];

/**
 * @brief 32-bits N_SHARES-shares masked values
 */
typedef uint32_t MSK_uint32_t[N_SHARES];

/**
 * @brief 64-bits N_SHARES-shares masked values
 */
typedef uint64_t MSK_uint64_t[N_SHARES];

/**********************************************************************
 * Basic functions and gadgets on masked values                    *
 **********************************************************************/

/**
 * @brief Generate 8-bits N_SHARES-shares boolean-masked version of unmasked
 * value
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked value
 * @param[in] a 8-bits unmasked value
 */
void MSK_8b_mask(MSK_uint8_t x, const uint8_t a);

/**
 * @brief Generate 16-bits N_SHARES-shares boolean-masked version of unmasked
 * value
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value
 * @param[in] a 16-bits unmasked value
 */
void MSK_16b_mask(MSK_uint16_t x, const uint16_t a);

/**
 * @brief Generate 32-bits N_SHARES-shares boolean-masked version of unmasked
 * value
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value
 * @param[in] a 32-bits unmasked value
 */
void MSK_32b_mask(MSK_uint32_t x, const uint32_t a);

/**
 * @brief Generate 64-bits N_SHARES-shares boolean-masked version of unmasked
 * value
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value
 * @param[in] a 64-bits unmasked value
 */
void MSK_64b_mask(MSK_uint64_t x, const uint64_t a);

/**
 * @brief Generate 8-bits N_SHARES-shares boolean-masked version of array of n
 * unmasked 8-bits values
 *
 * @param[out] [x] Array of n 8-bits N_SHARES-shares boolean-masked values
 * @param[in] a Array of n 8-bits unmasked value
 * @param[in] n Size of arrays
 */
void MSK_8b_mask_array(MSK_uint8_t x[], const uint8_t *const a, const size_t n);

/**
 * @brief Generate 16-bits N_SHARES-shares boolean-masked version of array of n
 * unmasked 16-bits values
 *
 * @param[out] [x] Array of n 16-bits N_SHARES-shares boolean-masked values
 * @param[in] a Array of n 16-bits unmasked value
 * @param[in] n Size of arrays
 */
void MSK_16b_mask_array(MSK_uint16_t x[], const uint16_t *const a,
                        const size_t n);

/**
 * @brief Generate 32-bits N_SHARES-shares boolean-masked version of array of n
 * unmasked 32-bits values
 *
 * @param[out] [x] Array of n 32-bits N_SHARES-shares boolean-masked values
 * @param[in] a Array of n 32-bits unmasked value
 * @param[in] n Size of arrays
 */
void MSK_32b_mask_array(MSK_uint32_t x[], const uint32_t *const a,
                        const size_t n);

/**
 * @brief Generate 64-bits N_SHARES-shares boolean-masked version of array of n
 * unmasked 64-bits values
 *
 * @param[out] [x] Array of n 64-bits N_SHARES-shares boolean-masked values
 * @param[in] a Array of n 64-bits unmasked value
 * @param[in] n Size of arrays
 */
void MSK_64b_mask_array(MSK_uint64_t x[], const uint64_t *const a,
                        const size_t n);

/**
 * @brief Unmask a 8-bits N_SHARES-shares boolean-masked value
 *
 * @return 8-bits unmasked value
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked value
 */
uint8_t MSK_8b_unmask(const MSK_uint8_t a);

/**
 * @brief Unmask a 16-bits N_SHARES-shares boolean-masked value
 *
 * @return 16-bits unmasked value
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked value
 */
uint16_t MSK_16b_unmask(const MSK_uint16_t a);

/**
 * @brief Unmask a 32-bits N_SHARES-shares boolean-masked value
 *
 * @return 32-bits unmasked value
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked value
 */
uint32_t MSK_32b_unmask(const MSK_uint32_t a);

/**
 * @brief Unmask a 64-bits N_SHARES-shares boolean-masked value
 *
 * @return 64-bits unmasked value
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked value
 */
uint64_t MSK_64b_unmask(const MSK_uint64_t a);

/**
 * @brief Unmask an array of n 8-bits N_SHARES-shares boolean-masked
 * values
 *
 * @param[out] x array of n 8-bits unmasked values
 * @param[in] [a] array of n 8-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_8b_unmask_array(uint8_t *const x, const MSK_uint8_t *const a,
                         const size_t n);

/**
 * @brief Unmask an array of n 16-bits N_SHARES-shares boolean-masked
 * values
 *
 * @param[out] x array of n 16-bits unmasked values
 * @param[in] [a] array of n 16-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_16b_unmask_array(uint16_t *const x, const MSK_uint16_t *const a,
                          const size_t n);

/**
 * @brief Unmask an array of n 32-bits N_SHARES-shares boolean-masked
 * values
 *
 * @param[out] x array of n 32-bits unmasked values
 * @param[in] [a] array of n 32-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_32b_unmask_array(uint32_t *const x, const MSK_uint32_t *const a,
                          const size_t n);

/**
 * @brief Unmask an array of n 64-bits N_SHARES-shares boolean-masked
 * values
 *
 * @param[out] x array of n 64-bits unmasked values
 * @param[in] [a] array of n 64-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_64b_unmask_array(uint64_t *const x, const MSK_uint64_t *const a,
                          const size_t n);

/**
 * @brief In-place recursive refresh of a 8-bits N_SHARES-shares boolean-masked
 * value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 8-bits N_SHARES-shares boolean-masked value
 */
void MSK_SNI_8b_inplace_refresh(MSK_uint8_t x);

/**
 * @brief In-place recursive refresh of a 16-bits N_SHARES-shares boolean-masked
 * value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 16-bits N_SHARES-shares boolean-masked value
 */
void MSK_SNI_16b_inplace_refresh(MSK_uint16_t x);

/**
 * @brief In-place recursive refresh of a 32-bits N_SHARES-shares boolean-masked
 * value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 32-bits N_SHARES-shares boolean-masked value
 */
void MSK_SNI_32b_inplace_refresh(MSK_uint32_t x);

/**
 * @brief In-place recursive refresh of a 64-bits N_SHARES-shares boolean-masked
 * value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 64-bits N_SHARES-shares boolean-masked value
 */
void MSK_SNI_64b_inplace_refresh(MSK_uint64_t x);

/**
 * @brief In-place refresh of an array of n 8-bits N_SHARES-shares
 * boolean-masked values
 *
 * @param[inout] [x] array of n 8-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_SNI_8b_inplace_refresh_array(MSK_uint8_t x[], const size_t n);

/**
 * @brief In-place refresh of an array of n 16-bits N_SHARES-shares
 * boolean-masked values
 *
 * @param[inout] [x] array of n 16-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_SNI_16b_inplace_refresh_array(MSK_uint16_t x[], const size_t n);

/**
 * @brief In-place refresh of an array of n 32-bits N_SHARES-shares
 * boolean-masked values
 *
 * @param[inout] [x] array of n 32-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_SNI_32b_inplace_refresh_array(MSK_uint32_t x[], const size_t n);

/**
 * @brief In-place refresh of an array of n 64-bits N_SHARES-shares
 * boolean-masked values
 *
 * @param[inout] [x] array of n 64-bits N_SHARES-shares boolean-masked values
 * @param[in] n number of elements in array a
 */
void MSK_SNI_64b_inplace_refresh_array(MSK_uint64_t x[], const size_t n);

/**
 * @brief In-place recursive refresh of a 8-bits N_SHARES-shares
 * arithmetic-masked value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 8-bits N_SHARES-shares arithmetic-masked value
 */
void MSKA_SNI_8b_inplace_refresh(MSK_uint8_t x);

/**
 * @brief In-place recursive refresh of a 16-bits N_SHARES-shares
 * arithmetic-masked value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 16-bits N_SHARES-shares arithmetic-masked value
 */
void MSKA_SNI_16b_inplace_refresh(MSK_uint16_t x);

/**
 * @brief In-place recursive refresh of a 32-bits N_SHARES-shares
 * arithmetic-masked value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 32-bits N_SHARES-shares arithmetic-masked value
 */
void MSKA_SNI_32b_inplace_refresh(MSK_uint32_t x);

/**
 * @brief In-place recursive refresh of a 64-bits N_SHARES-shares
 * arithmetic-masked value
 *
 * Algorithm from Battistello et al.
 *
 * @param[inout] [x] 64-bits N_SHARES-shares arithmetic-masked value
 */
void MSKA_SNI_64b_inplace_refresh(MSK_uint64_t x);

/**
 * @brief Inplace bitwise NOT of one 8-bits N_SHARES-shares boolean-masked value
 *
 * @param[inout] [x] 8-bits N_SHARES-shares boolean-masked value
 */
void MSK_8b_inplace_not(MSK_uint8_t x);

/**
 * @brief Inplace bitwise NOT of one 16-bits N_SHARES-shares boolean-masked
 * value
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value
 */
void MSK_16b_inplace_not(MSK_uint16_t x);

/**
 * @brief Inplace bitwise NOT of one 32-bits N_SHARES-shares boolean-masked
 * value
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value
 */
void MSK_32b_inplace_not(MSK_uint32_t x);

/**
 * @brief Inplace bitwise NOT of one 64-bits N_SHARES-shares boolean-masked
 * value
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value
 */
void MSK_64b_inplace_not(MSK_uint64_t x);

/**
 * @brief Bitwise NOT of one 8-bits N_SHARES-shares boolean-masked value
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise NOT of a
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked operand
 */
void MSK_8b_not(MSK_uint8_t x, const MSK_uint8_t a);

/**
 * @brief Bitwise NOT of one 16-bits N_SHARES-shares boolean-masked value
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise NOT of a
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked operand
 */
void MSK_16b_not(MSK_uint16_t x, const MSK_uint16_t a);

/**
 * @brief Bitwise NOT of one 32-bits N_SHARES-shares boolean-masked value
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise NOT of a
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked operand
 */
void MSK_32b_not(MSK_uint32_t x, const MSK_uint32_t a);

/**
 * @brief Bitwise NOT of one 64-bits N_SHARES-shares boolean-masked value
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise NOT of a
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked operand
 */
void MSK_64b_not(MSK_uint64_t x, const MSK_uint64_t a);

/**
 * @brief Bitwise XOR of two 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_8b_xor(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Bitwise XOR of two 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_16b_xor(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Bitwise XOR of two 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_32b_xor(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Bitwise XOR of two 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_64b_xor(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b);

/**
 * @brief Bitwise XOR of three 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 8-bits N_SHARES-shares boolean-masked third operand
 */
void MSK_8b_xor3(MSK_uint8_t r, const MSK_uint8_t a, const MSK_uint8_t b,
                 const MSK_uint8_t c);

/**
 * @brief Bitwise XOR of three 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 16-bits N_SHARES-shares boolean-masked third operand
 */
void MSK_16b_xor3(MSK_uint16_t r, const MSK_uint16_t a, const MSK_uint16_t b,
                  const MSK_uint16_t c);

/**
 * @brief Bitwise XOR of three 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 32-bits N_SHARES-shares boolean-masked third operand
 */
void MSK_32b_xor3(MSK_uint32_t r, const MSK_uint32_t a, const MSK_uint32_t b,
                  const MSK_uint32_t c);

/**
 * @brief Bitwise XOR of three 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 64-bits N_SHARES-shares boolean-masked third operand
 */
void MSK_64b_xor3(MSK_uint64_t r, const MSK_uint64_t a, const MSK_uint64_t b,
                  const MSK_uint64_t c);

/**
 * @brief Bitwise XOR of four 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 8-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 8-bits N_SHARES-shares boolean-masked fourth operand
 */
void MSK_8b_xor4(MSK_uint8_t r, const MSK_uint8_t a, const MSK_uint8_t b,
                 const MSK_uint8_t c, const MSK_uint8_t d);

/**
 * @brief Bitwise XOR of four 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 16-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 16-bits N_SHARES-shares boolean-masked fourth operand
 */
void MSK_16b_xor4(MSK_uint16_t r, const MSK_uint16_t a, const MSK_uint16_t b,
                  const MSK_uint16_t c, const MSK_uint16_t d);

/**
 * @brief Bitwise XOR of four 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 32-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 32-bits N_SHARES-shares boolean-masked fourth operand
 */
void MSK_32b_xor4(MSK_uint32_t r, const MSK_uint32_t a, const MSK_uint32_t b,
                  const MSK_uint32_t c, const MSK_uint32_t d);

/**
 * @brief Bitwise XOR of four 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 64-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 64-bits N_SHARES-shares boolean-masked fourth operand
 */
void MSK_64b_xor4(MSK_uint64_t r, const MSK_uint64_t a, const MSK_uint64_t b,
                  const MSK_uint64_t c, const MSK_uint64_t d);

/**
 * @brief Bitwise XOR of five 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 8-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 8-bits N_SHARES-shares boolean-masked fourth operand
 * @param[in] [e] 8-bits N_SHARES-shares boolean-masked fifth operand
 */
void MSK_8b_xor5(MSK_uint8_t r, const MSK_uint8_t a, const MSK_uint8_t b,
                 const MSK_uint8_t c, const MSK_uint8_t d, const MSK_uint8_t e);

/**
 * @brief Bitwise XOR of five 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 16-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 16-bits N_SHARES-shares boolean-masked fourth operand
 * @param[in] [e] 16-bits N_SHARES-shares boolean-masked fifth operand
 */
void MSK_16b_xor5(MSK_uint16_t r, const MSK_uint16_t a, const MSK_uint16_t b,
                  const MSK_uint16_t c, const MSK_uint16_t d,
                  const MSK_uint16_t e);

/**
 * @brief Bitwise XOR of five 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 32-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 32-bits N_SHARES-shares boolean-masked fourth operand
 * @param[in] [e] 32-bits N_SHARES-shares boolean-masked fifth operand
 */
void MSK_32b_xor5(MSK_uint32_t r, const MSK_uint32_t a, const MSK_uint32_t b,
                  const MSK_uint32_t c, const MSK_uint32_t d,
                  const MSK_uint32_t e);

/**
 * @brief Bitwise XOR of five 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise XOR of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 64-bits N_SHARES-shares boolean-masked third operand
 * @param[in] [d] 64-bits N_SHARES-shares boolean-masked fourth operand
 * @param[in] [e] 64-bits N_SHARES-shares boolean-masked fifth operand
 */
void MSK_64b_xor5(MSK_uint64_t r, const MSK_uint64_t a, const MSK_uint64_t b,
                  const MSK_uint64_t c, const MSK_uint64_t d,
                  const MSK_uint64_t e);

/**
 * @brief Bitwise and of a 8-bits N_SHARES-shares boolean-masked value and a
 * 8-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 8-bits unmasked second operand
 */
void MSK_8b_semi_and(MSK_uint8_t x, const MSK_uint8_t a, const uint8_t b);

/**
 * @brief Bitwise and of a 16-bits N_SHARES-shares boolean-masked value and a
 * 16-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 16-bits unmasked second operand
 */
void MSK_16b_semi_and(MSK_uint16_t x, const MSK_uint16_t a, const uint16_t b);

/**
 * @brief Bitwise and of a 32-bits N_SHARES-shares boolean-masked value and a
 * 32-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 32-bits unmasked second operand
 */
void MSK_32b_semi_and(MSK_uint32_t x, const MSK_uint32_t a, const uint32_t b);

/**
 * @brief Bitwise and of a 64-bits N_SHARES-shares boolean-masked value and a
 * 64-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 64-bits unmasked second operand
 */
void MSK_64b_semi_and(MSK_uint64_t x, const MSK_uint64_t a, const uint64_t b);

/**
 * @brief Bitwise AND of two 8-bits n-shares boolean-masked values
 *
 * @param[out] [x] 8-bits n-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 8-bits n-shares boolean-masked first operand
 * @param[in] [b] 8-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_SNI_8b_and(uint8_t *const x, const uint8_t *const a,
                     const uint8_t *const b, const size_t n);

/**
 * @brief Bitwise AND of two 16-bits n-shares boolean-masked values
 *
 * @param[out] [x] 16-bits n-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 16-bits n-shares boolean-masked first operand
 * @param[in] [b] 16-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_SNI_16b_and(uint16_t *const x, const uint16_t *const a,
                      const uint16_t *const b, const size_t n);

/**
 * @brief Bitwise AND of two 32-bits n-shares boolean-masked values
 *
 * @param[out] [x] 32-bits n-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 32-bits n-shares boolean-masked first operand
 * @param[in] [b] 32-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_SNI_32b_and(uint32_t *const x, const uint32_t *const a,
                      const uint32_t *const b, const size_t n);

/**
 * @brief Bitwise AND of two 64-bits n-shares boolean-masked values
 *
 * @param[out] [x] 64-bits n-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 64-bits n-shares boolean-masked first operand
 * @param[in] [b] 64-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_SNI_64b_and(uint64_t *const x, const uint64_t *const a,
                      const uint64_t *const b, const size_t n);

/**
 * @brief Bitwise AND of two 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_8b_and(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Bitwise AND of two 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_16b_and(MSK_uint16_t x, const MSK_uint16_t a,
                     const MSK_uint16_t b);

/**
 * @brief Bitwise AND of two 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_32b_and(MSK_uint32_t x, const MSK_uint32_t a,
                     const MSK_uint32_t b);

/**
 * @brief Bitwise AND of two 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise AND of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_64b_and(MSK_uint64_t x, const MSK_uint64_t a,
                     const MSK_uint64_t b);

/**
 * @brief Bitwise OR of two 8-bits N_SHARES-shares boolean-masked values
 * Based on: a OR b = NOT((NOT a) AND (NOT b))
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked bitwise OR of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_8b_or(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Bitwise OR of two 16-bits N_SHARES-shares boolean-masked values
 * Based on: a OR b = NOT((NOT a) AND (NOT b))
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked bitwise OR of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_16b_or(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Bitwise OR of two 32-bits N_SHARES-shares boolean-masked values
 * Based on: a OR b = NOT((NOT a) AND (NOT b))
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked bitwise OR of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_32b_or(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Bitwise OR of two 64-bits N_SHARES-shares boolean-masked values
 * Based on: a OR b = NOT((NOT a) AND (NOT b))
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked bitwise OR of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_SNI_64b_or(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b);

/**
 * @brief Right shift of a 8-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_8b_right_shift(MSK_uint8_t x, const MSK_uint8_t a, const size_t n);

/**
 * @brief Right shift of a 16-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_16b_right_shift(MSK_uint16_t x, const MSK_uint16_t a, const size_t n);

/**
 * @brief Right shift of a 32-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_32b_right_shift(MSK_uint32_t x, const MSK_uint32_t a, const size_t n);

/**
 * @brief Right shift of a 64-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_64b_right_shift(MSK_uint64_t x, const MSK_uint64_t a, const size_t n);

/**
 * @brief Left shift of a 8-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_8b_left_shift(MSK_uint8_t x, const MSK_uint8_t a, const size_t n);

/**
 * @brief Left shift of a 16-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_16b_left_shift(MSK_uint16_t x, const MSK_uint16_t a, const size_t n);

/**
 * @brief Left shift of a 32-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_32b_left_shift(MSK_uint32_t x, const MSK_uint32_t a, const size_t n);

/**
 * @brief Left shift of a 64-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] n shift amount
 */
void MSK_64b_left_shift(MSK_uint64_t x, const MSK_uint64_t a, const size_t n);

/**
 * @brief Left rotate of a 8-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_8b_left_rotate(MSK_uint8_t x, const MSK_uint8_t a, const size_t n);

/**
 * @brief Left rotate of a 16-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_16b_left_rotate(MSK_uint16_t x, const MSK_uint16_t a, const size_t n);

/**
 * @brief Left rotate of a 32-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_32b_left_rotate(MSK_uint32_t x, const MSK_uint32_t a, const size_t n);

/**
 * @brief Left rotate of a 64-bits N_SHARES-shares boolean-masked value by n
 * positions
 *  @param[out] [x] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] [a] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_64b_left_rotate(MSK_uint64_t x, const MSK_uint64_t a, const size_t n);

/**
 * @brief In-place left rotate of a 8-bits N_SHARES-shares boolean-masked value
 * by n positions
 *  @param[inout] [x] 8-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_8b_inplace_left_rotate(MSK_uint8_t x, const size_t n);

/**
 * @brief In-place left rotate of a 16-bits N_SHARES-shares boolean-masked value
 * by n positions
 *  @param[inout] [x] 16-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_16b_inplace_left_rotate(MSK_uint16_t x, const size_t n);

/**
 * @brief In-place left rotate of a 32-bits N_SHARES-shares boolean-masked value
 * by n positions
 *  @param[inout] [x] 32-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_32b_inplace_left_rotate(MSK_uint32_t x, const size_t n);

/**
 * @brief In-place left rotate of a 64-bits N_SHARES-shares boolean-masked value
 * by n positions
 *  @param[inout] [x] 64-bits N_SHARES-shares boolean-masked value
 *  @param[in] n rotate amount
 */
void MSK_64b_inplace_left_rotate(MSK_uint64_t x, const size_t n);

/**
 * @brief Addition of a 8-bits N_SHARES-shares boolean-masked value and a
 * 8-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 8-bits unmasked second operand
 */
void MSK_8b_semi_add(MSK_uint8_t x, const MSK_uint8_t a, const uint8_t b);

/**
 * @brief Addition of a 16-bits N_SHARES-shares boolean-masked value and a
 * 16-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 16-bits unmasked second operand
 */
void MSK_16b_semi_add(MSK_uint16_t x, const MSK_uint16_t a, const uint16_t b);

/**
 * @brief Addition of a 32-bits N_SHARES-shares boolean-masked value and a
 * 32-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 32-bits unmasked second operand
 */
void MSK_32b_semi_add(MSK_uint32_t x, const MSK_uint32_t a, const uint32_t b);

/**
 * @brief Addition of a 64-bits N_SHARES-shares boolean-masked value and a
 * 64-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 64-bits unmasked second operand
 */
void MSK_64b_semi_add(MSK_uint64_t x, const MSK_uint64_t a, const uint64_t b);

/**
 * @brief Addition of two 8-bits n-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 8-bits n-shares boolean-masked sum of a and b
 * @param[in] [a] 8-bits n-shares boolean-masked first operand
 * @param[in] [b] 8-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_8b_add(uint8_t *const x, const uint8_t *const a,
                 const uint8_t *const b, const size_t n);

/**
 * @brief Addition of two 16-bits n-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 16-bits n-shares boolean-masked sum of a and b
 * @param[in] [a] 16-bits n-shares boolean-masked first operand
 * @param[in] [b] 16-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_16b_add(uint16_t *const x, const uint16_t *const a,
                  const uint16_t *const b, const size_t n);

/**
 * @brief Addition of two 32-bits n-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 32-bits n-shares boolean-masked sum of a and b
 * @param[in] [a] 32-bits n-shares boolean-masked first operand
 * @param[in] [b] 32-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_32b_add(uint32_t *const x, const uint32_t *const a,
                  const uint32_t *const b, const size_t n);

/**
 * @brief Addition of two 64-bits n-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 64-bits n-shares boolean-masked sum of a and b
 * @param[in] [a] 64-bits n-shares boolean-masked first operand
 * @param[in] [b] 64-bits n-shares boolean-masked second operand
 * @param[in] n number of shares
 */
void _MSK_64b_add(uint64_t *const x, const uint64_t *const a,
                  const uint64_t *const b, const size_t n);

/**
 * @brief Addition of two 8-bits N_SHARES-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_8b_add(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Addition of two 16-bits N_SHARES-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_16b_add(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Addition of two 32-bits N_SHARES-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_32b_add(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Addition of two 64-bits N_SHARES-shares boolean-masked values
 * (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked sum of a and b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_64b_add(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b);

/**
* @brief Negative (in arithmetic sense) of a 8-bits N_SHARES-shares
boolean-masked value

* @param[out] [x] 8-bits N_SHARES-shares boolean-masked negative of a (x = -a)
* @param[in] [a] 8-bits N_SHARES-shares boolean-masked value
*/
void MSK_8b_neg(MSK_uint8_t x, const MSK_uint8_t a);

/**
* @brief Negative (in arithmetic sense) of a 16-bits N_SHARES-shares
boolean-masked value

* @param[out] [x] 16-bits N_SHARES-shares boolean-masked negative of a (x = -a)
* @param[in] [a] 16-bits N_SHARES-shares boolean-masked value
*/
void MSK_16b_neg(MSK_uint16_t x, const MSK_uint16_t a);

/**
* @brief Negative (in arithmetic sense) of a 32-bits N_SHARES-shares
boolean-masked value

* @param[out] [x] 32-bits N_SHARES-shares boolean-masked negative of a (x = -a)
* @param[in] [a] 32-bits N_SHARES-shares boolean-masked value
*/
void MSK_32b_neg(MSK_uint32_t x, const MSK_uint32_t a);

/**
* @brief Negative (in arithmetic sense) of a 64-bits N_SHARES-shares
boolean-masked value

* @param[out] [x] 64-bits N_SHARES-shares boolean-masked negative of a (x = -a)
* @param[in] [a] 64-bits N_SHARES-shares boolean-masked value
*/
void MSK_64b_neg(MSK_uint64_t x, const MSK_uint64_t a);

/**
 * @brief Subtraction of a 8-bits N_SHARES-shares boolean-masked value and a
 * 8-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked a - b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 8-bits unmasked second operand
 */
void MSK_8b_semi_sub(MSK_uint8_t x, const MSK_uint8_t a, const uint8_t b);

/**
 * @brief Subtraction of a 16-bits N_SHARES-shares boolean-masked value and a
 * 16-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked a - b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 16-bits unmasked second operand
 */
void MSK_16b_semi_sub(MSK_uint16_t x, const MSK_uint16_t a, const uint16_t b);

/**
 * @brief Subtraction of a 32-bits N_SHARES-shares boolean-masked value and a
 * 32-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked a - b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 32-bits unmasked second operand
 */
void MSK_32b_semi_sub(MSK_uint32_t x, const MSK_uint32_t a, const uint32_t b);

/**
 * @brief Subtraction of a 64-bits N_SHARES-shares boolean-masked value and a
 * 64-bits unmasked value (Algo 1 https://eprint.iacr.org/2014/891.pdf)
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked a - b
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] b 64-bits unmasked second operand
 */
void MSK_64b_semi_sub(MSK_uint64_t x, const MSK_uint64_t a, const uint64_t b);

/**
 * @brief Subtraction of two 8-bits N_SHARES-shares boolean-masked values
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked subtraction of a and b
 * (x = a - b)
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_8b_sub(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Subtraction of two 16-bits N_SHARES-shares boolean-masked values
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked subtraction of a and b
 * (x = a - b)
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_16b_sub(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Subtraction of two 32-bits N_SHARES-shares boolean-masked values
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked subtraction of a and b
 * (x = a - b)
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_32b_sub(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Subtraction of two 64-bits N_SHARES-shares boolean-masked values
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked subtraction of a and b
 * (x = a - b)
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 */
void MSK_64b_sub(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b);

/**
 * @brief Test equality of two 8-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked test result: 0 if a ==
 * b, else 1
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 *
 */
void MSK_8b_eq(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Test equality of two 16-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked test result: 0 if a ==
 * b, else 1
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 *
 */
void MSK_16b_eq(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Test equality of two 32-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked test result: 0 if a ==
 * b, else 1
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 *
 */
void MSK_32b_eq(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Test equality of two 64-bits N_SHARES-shares boolean-masked values
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked test result: 0 if a ==
 * b, else 1
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 *
 */
void MSK_64b_eq(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b);

/**
 * @brief Select one of two 8-bits N_SHARES-shares boolean-masked values,
 * based on the value of a third 8-bits boolean-masked value. The selection is
 * based on the parity of the third operand: the result is the first operand if
 * the third is even, else the result is the second operand.
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked result (x = (c % 2) ? b
 * : a)
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 8-bits N_SHARES-shares boolean-masked condition
 *
 */
void MSK_8b_if(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b,
               const MSK_uint8_t c);

/**
 * @brief Select one of two 16-bits N_SHARES-shares boolean-masked values,
 * based on the value of a third 16-bits boolean-masked value. The selection
 * is based on the parity of the third operand: the result is the first operand
 * if the third is even, else the result is the second operand.
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked result (x = (c % 2) ?
 * b : a)
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 16-bits N_SHARES-shares boolean-masked condition
 *
 */
void MSK_16b_if(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b,
                const MSK_uint16_t c);

/**
 * @brief Select one of two 32-bits N_SHARES-shares boolean-masked values,
 * based on the value of a third 32-bits boolean-masked value. The selection
 * is based on the parity of the third operand: the result is the first operand
 * if the third is even, else the result is the second operand.
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked result (x = (c % 2) ?
 * b : a)
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 32-bits N_SHARES-shares boolean-masked condition
 *
 */
void MSK_32b_if(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b,
                const MSK_uint32_t c);

/**
 * @brief Select one of two 64-bits N_SHARES-shares boolean-masked values,
 * based on the value of a third 64-bits boolean-masked value. The selection
 * is based on the parity of the third operand: the result is the first operand
 * if the third is even, else the result is the second operand.
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked result (x = (c % 2) ?
 * b : a)
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 64-bits N_SHARES-shares boolean-masked second operand
 * @param[in] [c] 64-bits N_SHARES-shares boolean-masked condition
 *
 */
void MSK_64b_if(MSK_uint64_t x, const MSK_uint64_t a, const MSK_uint64_t b,
                const MSK_uint64_t c);

/**
 * @brief Return the maximum of two 8-bits N_SHARES-shares boolean-masked
 *values, considered as unsigned numbers
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked maximum of a and b
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 8-bits N_SHARES-shares boolean-masked second operand
 **/
void MSK_8b_max(MSK_uint8_t x, const MSK_uint8_t a, const MSK_uint8_t b);

/**
 * @brief Return the maximum of two 16-bits N_SHARES-shares boolean-masked
 *values, considered as unsigned numbers
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked maximum of a and b
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 16-bits N_SHARES-shares boolean-masked second operand
 **/
void MSK_16b_max(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b);

/**
 * @brief Return the maximum of two 32-bits N_SHARES-shares boolean-masked
 *values, considered as unsigned numbers
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked maximum of a and b
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked first operand
 * @param[in] [b] 32-bits N_SHARES-shares boolean-masked second operand
 **/
void MSK_32b_max(MSK_uint32_t x, const MSK_uint32_t a, const MSK_uint32_t b);

/**
 * @brief Copy a 8-bits N_SHARES-shares boolean masked value to a 8-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked value.
 */
void MSK_8b_to_8b_copy(MSK_uint8_t x, const MSK_uint8_t a);

/**
 * @brief Copy a 8-bits N_SHARES-shares boolean masked value to a 16-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked value.
 */
void MSK_8b_to_16b_copy(MSK_uint16_t x, const MSK_uint8_t a);

/**
 * @brief Copy a 8-bits N_SHARES-shares boolean masked value to a 32-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked value.
 */
void MSK_8b_to_32b_copy(MSK_uint32_t x, const MSK_uint8_t a);

/**
 * @brief Copy a 8-bits N_SHARES-shares boolean masked value to a 64-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 8-bits N_SHARES-shares boolean-masked value.
 */
void MSK_8b_to_64b_copy(MSK_uint64_t x, const MSK_uint8_t a);

/**
 * @brief Copy a 16-bits N_SHARES-shares boolean masked value to a 8-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked value.
 */
void MSK_16b_to_8b_copy(MSK_uint8_t x, const MSK_uint16_t a);

/**
 * @brief Copy a 16-bits N_SHARES-shares boolean masked value to a 16-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked value.
 */
void MSK_16b_to_16b_copy(MSK_uint16_t x, const MSK_uint16_t a);

/**
 * @brief Copy a 16-bits N_SHARES-shares boolean masked value to a 32-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked value.
 */
void MSK_16b_to_32b_copy(MSK_uint32_t x, const MSK_uint16_t a);

/**
 * @brief Copy a 16-bits N_SHARES-shares boolean masked value to a 64-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 16-bits N_SHARES-shares boolean-masked value.
 */
void MSK_16b_to_64b_copy(MSK_uint64_t x, const MSK_uint16_t a);

/**
 * @brief Copy a 32-bits N_SHARES-shares boolean masked value to a 8-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked value.
 */
void MSK_32b_to_8b_copy(MSK_uint8_t x, const MSK_uint32_t a);

/**
 * @brief Copy a 32-bits N_SHARES-shares boolean masked value to a 16-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked value.
 */
void MSK_32b_to_16b_copy(MSK_uint16_t x, const MSK_uint32_t a);

/**
 * @brief Copy a 32-bits N_SHARES-shares boolean masked value to a 32-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked value.
 */
void MSK_32b_to_32b_copy(MSK_uint32_t x, const MSK_uint32_t a);

/**
 * @brief Copy a 32-bits N_SHARES-shares boolean masked value to a 64-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 32-bits N_SHARES-shares boolean-masked value.
 */
void MSK_32b_to_64b_copy(MSK_uint64_t x, const MSK_uint32_t a);

/**
 * @brief Copy a 64-bits N_SHARES-shares boolean masked value to a 8-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 8-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked value.
 */
void MSK_64b_to_8b_copy(MSK_uint8_t x, const MSK_uint64_t a);

/**
 * @brief Copy a 64-bits N_SHARES-shares boolean masked value to a 16-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 16-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked value.
 */
void MSK_64b_to_16b_copy(MSK_uint16_t x, const MSK_uint64_t a);

/**
 * @brief Copy a 64-bits N_SHARES-shares boolean masked value to a 32-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 32-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked value.
 */
void MSK_64b_to_32b_copy(MSK_uint32_t x, const MSK_uint64_t a);

/**
 * @brief Copy a 64-bits N_SHARES-shares boolean masked value to a 64-bits
 * N_SHARES-shares boolean masked value.
 *
 * @param[out] [x] 64-bits N_SHARES-shares boolean-masked value.
 * @param[in] [a] 64-bits N_SHARES-shares boolean-masked value.
 */
void MSK_64b_to_64b_copy(MSK_uint64_t x, const MSK_uint64_t a);

/**
 * @brief Copy size first elements of array a of 8-bits N_SHARES-shares boolean
 * masked values to array x of 8-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 8-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 8-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_8b_to_8b_copy_array(MSK_uint8_t x[], const MSK_uint8_t a[],
                             const size_t size);

/**
 * @brief Copy size first elements of array a of 8-bits N_SHARES-shares boolean
 * masked values to array x of 16-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 16-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 8-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_8b_to_16b_copy_array(MSK_uint16_t x[], const MSK_uint8_t a[],
                              const size_t size);

/**
 * @brief Copy size first elements of array a of 8-bits N_SHARES-shares boolean
 * masked values to array x of 32-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 32-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 8-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_8b_to_32b_copy_array(MSK_uint32_t x[], const MSK_uint8_t a[],
                              const size_t size);

/**
 * @brief Copy size first elements of array a of 8-bits N_SHARES-shares boolean
 * masked values to array x of 64-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 64-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 8-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_8b_to_64b_copy_array(MSK_uint64_t x[], const MSK_uint8_t a[],
                              const size_t size);
/**
 * @brief Copy size first elements of array a of 16-bits N_SHARES-shares boolean
 * masked values to array x of 8-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 8-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 16-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_16b_to_8b_copy_array(MSK_uint8_t x[], const MSK_uint16_t a[],
                              const size_t size);

/**
 * @brief Copy size first elements of array a of 16-bits N_SHARES-shares boolean
 * masked values to array x of 16-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 16-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 16-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_16b_to_16b_copy_array(MSK_uint16_t x[], const MSK_uint16_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 16-bits N_SHARES-shares boolean
 * masked values to array x of 32-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 32-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 16-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_16b_to_32b_copy_array(MSK_uint32_t x[], const MSK_uint16_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 16-bits N_SHARES-shares boolean
 * masked values to array x of 64-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 64-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 16-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_16b_to_64b_copy_array(MSK_uint64_t x[], const MSK_uint16_t a[],
                               const size_t size);
/**
 * @brief Copy size first elements of array a of 32-bits N_SHARES-shares boolean
 * masked values to array x of 8-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 8-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 32-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_32b_to_8b_copy_array(MSK_uint8_t x[], const MSK_uint32_t a[],
                              const size_t size);

/**
 * @brief Copy size first elements of array a of 32-bits N_SHARES-shares boolean
 * masked values to array x of 16-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 16-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 32-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_32b_to_16b_copy_array(MSK_uint16_t x[], const MSK_uint32_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 32-bits N_SHARES-shares boolean
 * masked values to array x of 32-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 32-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 32-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_32b_to_32b_copy_array(MSK_uint32_t x[], const MSK_uint32_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 32-bits N_SHARES-shares boolean
 * masked values to array x of 64-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 64-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 32-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_32b_to_64b_copy_array(MSK_uint64_t x[], const MSK_uint32_t a[],
                               const size_t size);
/**
 * @brief Copy size first elements of array a of 64-bits N_SHARES-shares boolean
 * masked values to array x of 8-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 8-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 64-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_64b_to_8b_copy_array(MSK_uint8_t x[], const MSK_uint64_t a[],
                              const size_t size);

/**
 * @brief Copy size first elements of array a of 64-bits N_SHARES-shares boolean
 * masked values to array x of 16-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 16-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 64-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_64b_to_16b_copy_array(MSK_uint16_t x[], const MSK_uint64_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 64-bits N_SHARES-shares boolean
 * masked values to array x of 32-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 32-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 64-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_64b_to_32b_copy_array(MSK_uint32_t x[], const MSK_uint64_t a[],
                               const size_t size);

/**
 * @brief Copy size first elements of array a of 64-bits N_SHARES-shares boolean
 * masked values to array x of 64-bits N_SHARES-shares boolean masked values.
 *
 * @param[out] [x] Destination array of 64-bits N_SHARES-shares boolean-masked
 * values.
 * @param[in] [a] Source array of 64-bits N_SHARES-shares boolean-masked values.
 * @param[in] size Number of elements to copy
 */
void MSK_64b_to_64b_copy_array(MSK_uint64_t x[], const MSK_uint64_t a[],
                               const size_t size);

/**********************************************************************
 * Conversions between arithletic and boolean masking (64 bits only)  *
 **********************************************************************/

/**
 * @brief Convert a 64-bits n-shares arithmetic-masked value to a 64-bits
 * n-shares boolean-masked value
 *
 * "Algorithm 4" from
 * http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf
 *
 * @param[out] [z] 64-bits n-shares boolean-masked value
 * @param[in] [a] 64-bits n-shares arithmetic-masked value
 * @param[in] n number of shares
 */
void MSK_arithmetic_to_boolean(uint64_t *const z, const uint64_t *const a,
                               const size_t n);

/**
 * @brief Convert a 64-bits n-shares boolean-masked value to a 64-bits n-shares
 * arithmetic-masked value
 *
 * "Algorithm 6" from
 * http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf
 *
 * @param[out] [z] 64-bits n-shares arithmetic-masked value
 * @param[in] [a] 64-bits n-shares boolean-masked value
 * @param[in] n Number of shares
 */
void MSK_boolean_to_arithmetic(uint64_t *const z, const uint64_t *const a,
                               const size_t n);

/**********************************************************************
 * Debugging helper functions                                         *
 **********************************************************************/

/* Debugging helper functions */

/**
 * @brief Print to fp the size first elements of array a of 8-bits unsigned
 * numbers. The elements are printed without separators, Least Significant Byte
 * first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] a array of 8-bits unsigned numbers to print
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void fprintf_uint8_t_array(FILE *fp, const char *const prefix,
                           const uint8_t *const a, const size_t size,
                           const char *const postfix);

/**
 * @brief Print to fp the size first elements of array a of 16-bits unsigned
 * numbers. The elements are printed without separators, Least Significant Byte
 * first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] a array of 16-bits unsigned numbers to print
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void fprintf_uint16_t_array(FILE *fp, const char *const prefix,
                            const uint16_t *const a, const size_t size,
                            const char *const postfix);

/**
 * @brief Print to fp the size first elements of array a of 32-bits unsigned
 * numbers. The elements are printed without separators, Least Significant Byte
 * first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] a array of 32-bits unsigned numbers to print
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void fprintf_uint32_t_array(FILE *fp, const char *const prefix,
                            const uint32_t *const a, const size_t size,
                            const char *const postfix);

/**
 * @brief Print to fp the size first elements of array a of 64-bits unsigned
 * numbers. The elements are printed without separators, Least Significant Byte
 * first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] a array of 64-bits unsigned numbers to print
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void fprintf_uint64_t_array(FILE *fp, const char *const prefix,
                            const uint64_t *const a, const size_t size,
                            const char *const postfix);

/**
 * @brief Unmask the size first elements of array a of 8-bits N_SHARES-shares
 * boolean-masked values and print to fp The elements are printed without
 * separators, Least Significant Byte first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] [a] array of 8-bits N_SHARES-shares boolean-masked values
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void MSK_8b_fprintf_array(FILE *fp, const char *const prefix,
                          const MSK_uint8_t *const a, const size_t size,
                          const char *const postfix);

/**
 * @brief Unmask the size first elements of array a of 16-bits N_SHARES-shares
 * boolean-masked values and print to fp The elements are printed without
 * separators, Least Significant Byte first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] [a] array of 16-bits N_SHARES-shares boolean-masked values
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void MSK_16b_fprintf_array(FILE *fp, const char *const prefix,
                           const MSK_uint16_t *const a, const size_t size,
                           const char *const postfix);

/**
 * @brief Unmask the size first elements of array a of 32-bits N_SHARES-shares
 * boolean-masked values and print to fp The elements are printed without
 * separators, Least Significant Byte first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] [a] array of 32-bits N_SHARES-shares boolean-masked values
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void MSK_32b_fprintf_array(FILE *fp, const char *const prefix,
                           const MSK_uint32_t *const a, const size_t size,
                           const char *const postfix);

/**
 * @brief Unmask the size first elements of array a of 64-bits N_SHARES-shares
 * boolean-masked values and print to fp The elements are printed without
 * separators, Least Significant Byte first
 *
 * @param[inout] fp file pointer to destination
 * @param[in] prefix string to prepend
 * @param[in] [a] array of 64-bits N_SHARES-shares boolean-masked values
 * @param[in] size number of elements to print
 * @param[in] postfix string to append
 */
void MSK_64b_fprintf_array(FILE *fp, const char *const prefix,
                           const MSK_uint64_t *const a, const size_t size,
                           const char *const postfix);

#endif
