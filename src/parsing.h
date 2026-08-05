/**
 * @file parsing.h
 * @brief Parsing of HQC secret keys, public keys and ciphertexts
 */

#ifndef MSK_PARSING_H
#define MSK_PARSING_H

#include <inttypes.h>
#include <string.h>

#include "mask.h"
#include "parameters.h"

/**
 * @brief Parse a public key into an array of bytes
 *
 * The public key is composed of the syndrome s and the seed used to generate
 * vector h
 *
 * @param[out] pk Public key, array of PUBLIC_KEY_BYTES bytes
 * @param[in] pk_seed Seed used to generate the public key, array of SEED_BYTES
 * bytes
 * @param[in] s Syndrome, array of VEC_N_SIZE_64 64-bits dwords
 */
void hqc_public_key_to_string(uint8_t pk[PUBLIC_KEY_BYTES],
                              const uint8_t pk_seed[SEED_BYTES],
                              const uint64_t s[VEC_N_SIZE_64]);

/**
 * @brief Parse a public key from an array of bytes
 *
 * The public key is composed of the syndrome s and the seed used to generate
 * vector h
 *
 * @param[out] [h] Vector h, array of VEC_N_SIZE_64 N_SHARES-shares 64-bits
 * boolean-masked values
 * @param[out] [s] Syndrome, array of VEC_N_SIZE_64 N_SHARES-shares 64-bits
 * boolean-masked values
 * @param[in] pk Public key, array of PUBLIC_KEY_BYTES bytes
 */
void MSK_hqc_public_key_from_string(MSK_uint64_t h[VEC_N_SIZE_64],
                                    MSK_uint64_t s[VEC_N_SIZE_64],
                                    const uint8_t pk[PUBLIC_KEY_BYTES]);

/**
 * @brief Parse a ciphertext into an array of bytes
 *
 * The ciphertext is composed of vectors u, v and salt.
 *
 * Inputs are masked but as the ciphertext is declassified it is unmasked before
 * emitting it
 *
 * @param[out] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 * @param[in] [mu] Masked vector u part of ciphertext, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[in] [mv] Masked vector v part of ciphertext, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[in] [msalt] Masked salt, array of SALT_SIZE_64 N_SHARES-shares 64-bits
 * boolean-masked values
 */
void MSK_hqc_ciphertext_to_string(uint8_t ct[CIPHERTEXT_BYTES],
                                  const MSK_uint64_t mu[VEC_N_SIZE_64],
                                  const MSK_uint64_t mv[VEC_N1N2_SIZE_64],
                                  const MSK_uint8_t msalt[SALT_SIZE_64]);

/**
 * @brief Parse a ciphertext from an array of bytes
 *
 * The ciphertext is composed of vectors u, v and salt.
 *
 * @param[out] [u] Masked vector u part of ciphertext, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[out] [v] Masked vector v part of ciphertext, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[out] [salt] Masked salt, array of SALT_SIZE_64 N_SHARES-shares 64-bits
 * boolean-masked values
 * @param[in] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 */
void MSK_hqc_ciphertext_from_string(MSK_uint64_t u[VEC_N_SIZE_64],
                                    MSK_uint64_t v[VEC_N1N2_SIZE_64],
                                    MSK_uint64_t salt[SALT_SIZE_64],
                                    const uint8_t ct[CIPHERTEXT_BYTES]);

/**
 * @brief Parse a secret key from an array of bytes
 *
 * The secret key is composed of the seed used to generate vectors x and y.
 *
 * @param[out] [y] Masked vector y part of secret key, array of VEC_N1_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[out] [sigma] String used for rejection in HHK transform, array of
 * VEC_K_SIZE_BYTES N_SHARES-shares boolean-masked bytes
 * @param[in] [sk] Secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes
 */
void MSK_hqc_secret_key_from_string(MSK_uint64_t y[VEC_N1_SIZE_64],
                                    MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                                    const MSK_uint8_t sk[SECRET_KEY_BYTES]);

/**
 * @brief Convert an array of N_SHARES-shares 64-bits boolean-masked values into
 * a flat array of bytes
 *
 * @param[out] dest Flat byte array equivalent of src input
 * @param[in] [src] Input array of N_SHARES-shares 64-bits boolean-masked values
 * @param[in] size64 Size of input array src
 */
void MSK_store64_to_8(uint8_t *dest, const MSK_uint64_t src[],
                      const size_t size64);

/**
 * @brief Parse a secret key into an array of bytes
 *
 * The secret key is composed of the seed used to generate vectors x and y. As
 * technicality, the public key is appended to the secret key in order to
 * respect NIST API.
 *
 * @param[out] [sk] Masked secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes
 * @param[in] [sk_seed] Seed used to generate the secret key, array of
 * SEED_BYTES N_SHARES-shares boolean-masked bytes
 * @param[in] [sigma] String used for rejection in HHK transform, array of
 * VEC_K_SIZE_BYTES N_SHARES-shares boolean-masked bytes
 * @param[in] pk Public key, array of PUBLIC_KEY_BYTES bytes
 */
void MSK_hqc_secret_key_to_string(MSK_uint8_t sk[SECRET_KEY_BYTES],
                                  const MSK_uint8_t sk_seed[SEED_BYTES],
                                  const MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                                  const uint8_t pk[PUBLIC_KEY_BYTES]);

#endif
