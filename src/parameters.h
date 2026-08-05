/**
 * @file parameters.h
 * @brief Parameters of the HQC_KEM IND-CCA2 scheme
 */

#ifndef MSK_HQC_PARAMETERS_H
#define MSK_HQC_PARAMETERS_H

#include <stdint.h>

#include "api.h"

/**
 * @brief Divide a by b and ceil the result
 */
#define CEIL_DIVIDE(a, b) (((a) / (b)) + ((a) % (b) == 0 ? 0 : 1))

/**
 * @brief Create a mask
 */
#define BITMASK(a, size) ((1UL << (a % size)) - 1)

/**
 * @brief Byte-length of secret key, including seed and public key
 */
#define SECRET_KEY_BYTES CRYPTO_SECRETKEYBYTES

/**
 * @brief Byte-length of public key
 */
#define PUBLIC_KEY_BYTES CRYPTO_PUBLICKEYBYTES

/**
 * @brief Byte-length of shared secret
 */
#define SHARED_SECRET_BYTES CRYPTO_BYTES

/**
 * @brief Byte-length of ciphertext
 */
#define CIPHERTEXT_BYTES CRYPTO_CIPHERTEXTBYTES

/**
 * @brief Dimension of Reed-Muller codes
 */
#define PARAM_M 8

/**
 * @brief Hexadecimal form of primitive polynomial of \f$GF_2^{PARAM\_M}\f$ used
 * for Reed-Solomon codes
 */
#define PARAM_GF_POLY 0x11D

/**
 * @brief Hamming weight of primitive polynomial
 */
#define PARAM_GF_POLY_WT 5

/**
 * @brief Distance between two first set bits of primitive polynomial
 */
#define PARAM_GF_POLY_M2 4

/**
 * @brief Size of \f$GF_2^{PARAM\_M}\f$ multiplicative group
 * (\f$2^{PARAM\_M}-1\f$)
 */
#define PARAM_GF_MUL_ORDER 255

/**
 * @brief Byte-length of SHAKE-256 output
 */
#define SHAKE256_512_BYTES 64

/**
 * @brief Byte-length of seed
 */
#define SEED_BYTES 40

/**
 * @brief Byte-length of salt
 */
#define SALT_SIZE_BYTES 16

/**
 * @brief Byte-length of salt in 64-bits dwords
 */
#define SALT_SIZE_64 2

#if SECURITY_LEVEL == 128

/**
 * @brief N parameter of scheme (dimension of the random code)
 */
#define PARAM_N 17669

/**
 * @brief Length of Reed-Solomon code
 */
#define PARAM_N1 46

/**
 * @brief Length of Duplicated Reed-Muller code
 */
#define PARAM_N2 384

/**
 * @brief Bit-length of concatenated code
 */
#define PARAM_N1N2 17664

/**
 * @brief Weight of the x and y vector parts of the secret key in HQC PKE scheme
 */
#define PARAM_OMEGA 66

/**
 * @brief Weight of the e random error used for encryption in HQC PKE scheme
 */
#define PARAM_OMEGA_E 75

/**
 * @brief Correcting capacity of the Reed-Solomon code
 */
#define PARAM_DELTA 15

/**
 * @brief Size of information bits of Reed-Solomon code
 */
#define PARAM_K 16

/**
 * @brief Size of generator polynomial of Reed-Solomon code
 */
#define PARAM_G 31

/**
 * @brief Log2 of size of FFT input polynomial
 */
#define PARAM_FFT 4

/**
 * @brief Coefficients of generator polynomial of Reed-Solomon code
 */
#define RS_POLY_COEFS                                                          \
  89, 69, 153, 116, 176, 117, 111, 75, 73, 233, 242, 233, 65, 210, 21, 139,    \
      103, 173, 67, 118, 105, 210, 174, 110, 74, 69, 228, 82, 255, 181, 1

/**
 * @brief Mask of vector high bits
 */
#define RED_MASK BITMASK(PARAM_N, 64)

#elif SECURITY_LEVEL == 192

/**
 * @brief N parameter of scheme (dimension of the random code)
 */
#define PARAM_N 35851

/**
 * @brief Length of Reed-Solomon code
 */
#define PARAM_N1 56

/**
 * @brief Length of Duplicated Reed-Muller code
 */
#define PARAM_N2 640

/**
 * @brief Bit-length of concatenated code
 */
#define PARAM_N1N2 35840

/**
 * @brief Weight of the x and y vector parts of the secret key in HQC PKE scheme
 */
#define PARAM_OMEGA 100

/**
 * @brief Weight of the e random error used for encryption in HQC PKE scheme
 */
#define PARAM_OMEGA_E 114

/**
 * @brief Correcting capacity of the Reed-Solomon code
 */
#define PARAM_DELTA 16

/**
 * @brief Size of information bits of Reed-Solomon code
 */
#define PARAM_K 24

/**
 * @brief Size of generator polynomial of Reed-Solomon code
 */
#define PARAM_G 33

/**
 * @brief Log2 of size of FFT input polynomial
 */
#define PARAM_FFT 5

/**
 * @brief Coefficients of generator polynomial of Reed-Solomon code
 */
#define RS_POLY_COEFS                                                          \
  45, 216, 239, 24, 253, 104, 27, 40, 107, 50, 163, 210, 227, 134, 224, 158,   \
      119, 13, 158, 1, 238, 164, 82, 43, 15, 232, 246, 142, 50, 189, 29, 232,  \
      1

/**
 * @brief Mask of vector high bits
 */
#define RED_MASK 0x7ff

#elif SECURITY_LEVEL == 256

/**
 * @brief N parameter of scheme (dimension of the random code)
 */
#define PARAM_N 57637

/**
 * @brief Length of Reed-Solomon code
 */
#define PARAM_N1 90

/**
 * @brief Length of Duplicated Reed-Muller code
 */
#define PARAM_N2 640

/**
 * @brief Bit-length of concatenated code
 */
#define PARAM_N1N2 57600

/**
 * @brief Weight of the x and y vector parts of the secret key in HQC PKE scheme
 */
#define PARAM_OMEGA 131

/**
 * @brief Weight of the e random error used for encryption in HQC PKE scheme
 */
#define PARAM_OMEGA_E 149

/**
 * @brief Correcting capacity of the Reed-Solomon code
 */
#define PARAM_DELTA 29

/**
 * @brief Size of information bits of Reed-Solomon code
 */
#define PARAM_K 32

/**
 * @brief Size of generator polynomial of Reed-Solomon code
 */
#define PARAM_G 59

/**
 * @brief Log2 of size of FFT input polynomial
 */
#define PARAM_FFT 5

/**
 * @brief Coefficients of generator polynomial of Reed-Solomon code
 */
#define RS_POLY_COEFS                                                          \
  49, 167, 49, 39, 200, 121, 124, 91, 240, 63, 148, 71, 150, 123, 87, 101, 32, \
      215, 159, 71, 201, 115, 97, 210, 186, 183, 141, 217, 123, 12, 31, 243,   \
      180, 219, 152, 239, 99, 141, 4, 246, 191, 144, 8, 232, 47, 27, 141, 178, \
      130, 64, 124, 47, 39, 188, 216, 48, 199, 187, 1

/**
 * @brief Mask of vector high bits
 */
#define RED_MASK 0x1fffffffff

#else

#error "Invalid security level"

#endif

/**
 * @brief Byte-length of a PARAM_N bits vector
 */
#define VEC_N_SIZE_BYTES CEIL_DIVIDE(PARAM_N, 8)

/**
 * @brief Byte-length of a PARAM_K bytes vector
 */
#define VEC_K_SIZE_BYTES PARAM_K

/**
 * @brief Byte-length of a PARAM_N1 bytes vector
 */
#define VEC_N1_SIZE_BYTES PARAM_N1

/**
 * @brief Byte-length of a PARAM_N1N2 bits vector
 */
#define VEC_N1N2_SIZE_BYTES CEIL_DIVIDE(PARAM_N1N2, 8)

/**
 * @brief 64-bits dword-length of a PARAM_N bits vector
 */
#define VEC_N_SIZE_64 CEIL_DIVIDE(PARAM_N, 64)

/**
 * @brief 64-bits dword-length of a PARAM_K bytes vector
 */
#define VEC_K_SIZE_64 CEIL_DIVIDE(PARAM_K, 8)

/**
 * @brief 64-bits dword-length of a PARAM_N1 bytes vector
 */
#define VEC_N1_SIZE_64 CEIL_DIVIDE(PARAM_N1, 8)

/**
 * @brief 64-bits dword-length of a PARAM_N1N2 bits vector
 */
#define VEC_N1N2_SIZE_64 CEIL_DIVIDE(PARAM_N1N2, 64)

/**
 * @brief Weight of the \f$r_1\f$ and \f$r_2\f$ random vectors used for
 * encryption in HQC PKE scheme
 *
 * Note: same as PARAM_OMEGA_E (weight of error)
 */
#define PARAM_OMEGA_R PARAM_OMEGA_E

#endif
