/**
 * @file api.h
 * @brief NIST KEM API used by the HQC_KEM IND-CCA2 scheme
 */

#ifndef API_H
#define API_H

#ifndef SECURITY_LEVEL
/**
 * @brief Default security level (HQC-128)
 */
#define SECURITY_LEVEL 128
#endif

#if SECURITY_LEVEL == 128

/**
 * @brief Algorithm name
 */
#define CRYPTO_ALGNAME "HQC-128"

/**
 * @brief Total byte size of secret key, including seed and public key
 */
#define CRYPTO_SECRETKEYBYTES 2305

/**
 * @brief Byte size of public key
 */
#define CRYPTO_PUBLICKEYBYTES 2249

/**
 * @brief Byte size of shared secret
 */
#define CRYPTO_BYTES 64

/**
 * @brief Byte size of ciphertext
 */
#define CRYPTO_CIPHERTEXTBYTES 4433

#elif SECURITY_LEVEL == 192

/**
 * @brief Algorithm name
 */
#define CRYPTO_ALGNAME "HQC-192"

/**
 * @brief Total byte size of secret key, including seed and public key
 */
#define CRYPTO_SECRETKEYBYTES 4586

/**
 * @brief Byte size of public key
 */
#define CRYPTO_PUBLICKEYBYTES 4522

/**
 * @brief Byte size of shared secret
 */
#define CRYPTO_BYTES 64

/**
 * @brief Byte size of ciphertext
 */
#define CRYPTO_CIPHERTEXTBYTES 8978

#elif SECURITY_LEVEL == 256

/**
 * @brief Algorithm name
 */
#define CRYPTO_ALGNAME "HQC-256"

/**
 * @brief Total byte size of secret key, including seed and public key
 */
#define CRYPTO_SECRETKEYBYTES 7317

/**
 * @brief Byte size of public key
 */
#define CRYPTO_PUBLICKEYBYTES 7245

/**
 * @brief Byte size of shared secret
 */
#define CRYPTO_BYTES 64

/**
 * @brief Byte size of ciphertext
 */
#define CRYPTO_CIPHERTEXTBYTES 14421

#else

#error "Invalid security level"

#endif

// As a technicality, the public key is appended to the secret key in order to
// respect the NIST API. Without this constraint, CRYPTO_SECRETKEYBYTES would be
// defined as 32

/**
 * @brief Unmasked key pair generation
 *
 * Call masked version, unmask generated secret key
 *
 * @param[out] pk Public key, array of PUBLIC_KEY_BYTES bytes
 * @param[out] sk Secret key, array of SECRET_KEY_BYTES bytes
 */
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk);

/**
 * @brief Unmasked encapsulation
 *
 * Call masked version, unmask encapsulated shared secret
 *
 * @param[out] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 * @param[out] ss Shared secret, array of SHARED_SECRET_BYTES bytes
 * @param[in] pk Public key, array of PUBLIC_KEY_BYTES bytes
 */
int crypto_kem_enc(unsigned char *ct, unsigned char *ss,
                   const unsigned char *pk);

/**
 * @brief Unmasked decapsulation
 *
 * Mask secret key, call masked version, unmask decapsulated shared secret
 *
 * @param[out] ss Shared secret, array of SHARED_SECRET_BYTES bytes
 * @param[in] ct Ciphertext, array of CIPHERTEXT_BYTES bytes
 * @param[in] sk Secret key, array of SECRET_KEY_BYTES bytes
 */
int crypto_kem_dec(unsigned char *ss, const unsigned char *ct,
                   const unsigned char *sk);

#endif
