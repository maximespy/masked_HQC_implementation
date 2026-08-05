/**
 * @file reed_solomon.h
 * @brief Masked implementation of Reed-Solomon codes
 */

#ifndef MSK_REED_SOLOMON_H
#define MSK_REED_SOLOMON_H

#include "mask.h"
#include "parameters.h"
#include <inttypes.h>

/**
 * @brief Encode a message to a Reed-Solomon code word
 *
 * Use classic vector-matrix multiplication c = mG. The generator matrix (static
 * constant declared in read_solomon.c) is not masked as the code is public.
 *
 * @param[out] [cdw] Encoded code word, array of PARAM_N1 N_SHARES-shares
 * boolean-masked bytes
 * @param[inout] [msg] Input message to encode, array of PARAM_K N_SHARES-shares
 * boolean-masked bytes (refreshed)
 */
void MSK_rs_encode(MSK_uint8_t cdw[PARAM_N1], MSK_uint8_t msg[PARAM_K]);

/**
 * @brief Decode a received code word
 *
 * The decoding is a 6-steps operation:
 *
 * <ol>
 *    <li> Compute \f$2 \times PARAM\_DELTA\f$ syndromes.
 *    <li> Compute error-locator polynomial sigma.
 *    <li> Find error-locator numbers by calculating roots of polynomial sigma
 *         and takings their inverses. Done using additive FFT.
 *    <li> Compute Z polynomial.
 *    <li> Compute error values.
 *    <li> Correction of errors in received polynomial.
 * </ol>
 *
 * For a more complete picture on Reed-Solomon decoding, see Shu. Lin and Daniel
 * J. Costello in Error Control Coding: Fundamentals and Applications @cite
 * lin1983error
 *
 * @param[out] [msg] Decoded message, array of PARAM_K N_SHARES-shares
 * boolean-masked bytes
 * @param[inout] [cdw] Received code word, array of PARAM_N1 N_SHARES-shares
 * boolean-masked bytes (refreshed)
 */
void MSK_reed_solomon_decode(MSK_uint8_t msg[PARAM_K],
                             MSK_uint8_t cdw[PARAM_N1]);

#endif
