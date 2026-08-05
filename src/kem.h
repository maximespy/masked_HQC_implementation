/**
 * @file kem.h
 * @brief Masked version of HQC Key Exchange Mechanism (KEM)
 */

#ifndef KEM_H
#define KEM_H

#include <inttypes.h>

#include "mask.h"
#include "parameters.h"

/**
 * @brief Keygen of the HQC_KEM IND_CAA2 scheme
 *
 * The public key is composed of the syndrome s and the seed used to generate
 * the vector h.
 *
 * The secret key is composed of the seed used to generate vectors x and
 * y. As a technicality, the public key is appended to the secret key in
 * order to respect NIST API.
 *
 * @param[out] pk Public key, array of PUBLIC_KEY_BYTES bytes
 * @param[out] [sk] Secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes
 * @returns 0 if keygen is successful
 */
int MSK_crypto_kem_keypair(uint8_t pk[PUBLIC_KEY_BYTES],
                           MSK_uint8_t sk[SECRET_KEY_BYTES]);

/**
 * @brief Encapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 * @param[out] [ss] Shared secret, array of SHARED_SECRET_BYTES N_SHARES-shares
 * boolean-masked bytes
 * @param[in] pk Public key, array of PUBLIC_KEY_BYTES bytes
 * @param[in] unmasked_salts If non-zero salts are generated unmasked
 * @returns 0 if encapsulation is successful
 */
int MSK_crypto_kem_enc(uint8_t ct[CIPHERTEXT_BYTES],
                       MSK_uint8_t ss[SHARED_SECRET_BYTES],
                       const uint8_t pk[PUBLIC_KEY_BYTES], int unmasked_salts);

/**
 * @brief Decapsulation of the HQC_KEM IND_CAA2 scheme
 *
 * @param[out] [ss] Shared secret, array of SHARED_SECRET_BYTES N_SHARES-shares
 * boolean-masked bytes
 * @param[out] [result] Masked decapsulation success (0) or failure (255)
 * @param[in] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 * @param[inout] [sk] Secret key, array of SECRET_KEY_BYTES N_SHARES-shares
 * boolean-masked bytes (refreshed)
 */
void MSK_crypto_kem_dec(MSK_uint8_t ss[SHARED_SECRET_BYTES], MSK_uint8_t result,
                        const uint8_t ct[CIPHERTEXT_BYTES],
                        MSK_uint8_t sk[SECRET_KEY_BYTES]);

#endif
