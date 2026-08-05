/**
 * @file fft.h
 * @brief Additive FFT and its transpose. Additive fast fourier transforms over
 * fnite felds.
 *
 * This implementation is based on the paper from Gao and Mateer:
 * Shuhong Gao and Todd Mateer, Additive Fast Fourier Transforms over Finite
 * Fields, IEEE Transactions on Information Theory 56 (2010), 6265--6272.
 * http://www.math.clemson.edu/~sgao/papers/GM10.pdf
 * and includes improvements proposed by Bernstein, Chou and Schwabe here:
 * https://binary.cr.yp.to/mcbits-20130616.pdf
 */

#ifndef MSK_FFT_H
#define MSK_FFT_H

#include "mask.h"
#include "parameters.h"

#include <inttypes.h>

/**
 * @brief Evaluate f on all fields elements with additive FFT
 *
 * The FFT proceeds recursively to evaluate f at all subset sums of a basis B.
 * On this top call (as opposed to the recursive calls to fft_rec), gammas are
 * equal to betas, meaning the first gammas subset sums are actually the subset
 * sums of betas (except 1). Note: f is altered during computation (twisted at
 * each level) and refreshed.
 *
 * @param[out] [w] FFT output, array of N_SHARES-shares 16-bits
 * boolean-masked values
 * @param[inout] [f] FFT input, array of N_SHARES-shares 16-bits
 * boolean-masked values (refreshed)
 * @param[in] f_coeffs Number of coefficients of f (deg(f) + 1)
 */
void MSK_fft(MSK_uint16_t w[], MSK_uint16_t f[], const size_t f_coeffs);

/**
 * @brief Retrieve error polynomial from evaluations w of Error Locator
 * Polynomial (ELP) on all field elements
 *
 * @param[out] [error] Error polynomial, array of \f$2^{PARAM\_M}\f$
 * N_SHARES-shares 16-bits boolean-masked values
 * @param[inout] [w] FFT output, array of \f$2^{PARAM\_M}\f$ N_SHARES-shares
 * 16-bits boolean-masked values (refreshed)
 */
void MSK_fft_retrieve_error_poly(MSK_uint8_t error[1 << PARAM_M],
                                 MSK_uint16_t w[1 << PARAM_M]);

#endif
