/**
 * @file hqc.h
 * @brief Masked HQC Public Key Encryption (PKE)
 */

#ifndef MSK_HQC_H
#define MSK_HQC_H

#include <inttypes.h>

#include "mask.h"
#include "parameters.h"

/**
 * @brief Masked key generation of the HQC_PKE IND_CPA scheme
 *
 * The public key contains syndrome s and the seed used to
 * generate vector h. The secret key is the seed used to generate
 * vectors x and y. As a technicality, the public key is appended
 * to the secret key in order to respect NIST API.
 *
 * @param[out] pk public key, array of PUBLIC_KEY_BYTES bytes
 * @param[out] [sk] secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes
 */
void MSK_hqc_pke_keygen(uint8_t pk[PUBLIC_KEY_BYTES],
                        MSK_uint8_t sk[SECRET_KEY_BYTES]);

/**
 * @brief Masked encryption of the HQC_PKE IND_CPA scheme
 *
 * The output cihertext is the pair of vectors u and v.
 *
 * @param[out] [u] First ciphertext component, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[out] [v] Second ciphertext component, array of VEC_N1N2_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[inout] [m] Boolean-masked message to encrypt, array of
 * VEC_K_SIZE_BYTES N_SHARES-shares boolean-masked bytes (refreshed)
 * @param[in] [theta] Seed used to derive randomness required for encryption,
 * array of SHAKE-256_512_BYTES N_SHARES-shares boolean-masked bytes
 * @param[in] pk Public key, array of PUBLIC_KEY_BYTES bytes
 */
void MSK_hqc_pke_encrypt(MSK_uint64_t u[VEC_N_SIZE_64],
                         MSK_uint64_t v[VEC_N1N2_SIZE_64],
                         MSK_uint8_t m[VEC_K_SIZE_BYTES],
                         MSK_uint8_t theta[SHAKE256_512_BYTES],
                         const uint8_t pk[PUBLIC_KEY_BYTES]);

/**
 * @brief Masked decryption of the HQC_PKE IND_CPA scheme
 *
 * @param[out] [m] Decrypted message, array of VEC_K_SIZE_BYTES
 * N_SHARES-shares boolean-masked bytes
 * @param[out] [sigma] String used for rejection in HHK transform, array of
 * VEC_K_SIZE_BYTES N_SHARES-shares boolean-masked bytes
 * @param[inout] [u] First ciphertext component, array of VEC_N_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values (refreshed)
 * @param[in] [v] Second ciphertext component, array of VEC_N1N2_SIZE_64
 * N_SHARES-shares 64-bits boolean-masked values
 * @param[inout] [sk] Secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes (refreshed)
 * @returns 0
 */
uint8_t MSK_hqc_pke_decrypt(MSK_uint8_t m[VEC_K_SIZE_BYTES],
                            MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                            MSK_uint64_t u[VEC_N_SIZE_64],
                            const MSK_uint64_t v[VEC_N1N2_SIZE_64],
                            MSK_uint8_t sk[SECRET_KEY_BYTES]);

#endif
