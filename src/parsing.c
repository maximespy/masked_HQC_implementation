/**
 * @file parsing.c
 * @brief Functions to parse secret key, public key and ciphertext of the HQC
 * scheme
 */

#include "parsing.h"
#include "shake_prng.h"
#include "vector.h"

/**
 * @brief Convert an 8-bytes array to uint64_t
 *
 * First byte of input array is least significant byte of result
 *
 * @param[in] in Input array of 8 bytes
 * @return 64-bits value
 */
static uint64_t load8(const uint8_t *const in) {
  uint64_t ret = in[7];

  for (int i = 6; i >= 0; --i) {
    ret <<= 8;
    ret |= in[i];
  }

  return ret;
}

/**
 * @brief Convert an array of bytes into an array of uint64_t
 *
 * First byte of input array is least significant byte of first 64 bits word of
 * result
 *
 * @param[out] out64 Output array of outlen 64-bits values
 * @param[in] outlen Number of elements of output array
 * @param[in] in8 Input array of inlen bytes
 * @param[in] inlen Number of elements of input array
 */
void load8_arr(uint64_t *const out64, const size_t outlen,
               const uint8_t *const in8, const size_t inlen) {
  size_t index_in = 0;
  size_t index_out = 0;

  // first copy by 8 bytes
  if (inlen >= 8 && outlen >= 1) {
    while (index_out < outlen && index_in + 8 <= inlen) {
      out64[index_out] = load8(in8 + index_in);

      index_in += 8;
      index_out += 1;
    }
  }

  // we now need to do the last 7 bytes if necessary
  if (index_in >= inlen || index_out >= outlen) {
    return;
  }
  out64[index_out] = in8[inlen - 1];
  for (int8_t i = (int8_t)(inlen - index_in) - 2; i >= 0; --i) {
    out64[index_out] <<= 8;
    out64[index_out] |= in8[index_in + i];
  }
}

/**
 * @brief Convert an array of uint64_t into an array of bytes
 *
 * First byte of output array is least significant byte of first 64 bits word of
 * input
 *
 * @param[out] out8 Output array of outlen bytes
 * @param[in] outlen Number of elements of output array
 * @param[in] in64 Input array of inlen 64-bits values
 * @param[in] inlen Number of elements of input array
 */
void store8_arr(uint8_t *const out8, const size_t outlen,
                const uint64_t *const in64, const size_t inlen) {
  for (size_t index_out = 0, index_in = 0;
       index_out < outlen && index_in < inlen;) {
    out8[index_out] = (in64[index_in] >> ((index_out % 8) * 8)) & 0xFF;
    ++index_out;
    if (index_out % 8 == 0) {
      ++index_in;
    }
  }
}

void MSK_hqc_secret_key_to_string(MSK_uint8_t sk[SECRET_KEY_BYTES],
                                  const MSK_uint8_t sk_seed[SEED_BYTES],
                                  const MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                                  const uint8_t pk[PUBLIC_KEY_BYTES]) {
  MSK_8b_to_8b_copy_array(sk, sk_seed, SEED_BYTES);
  MSK_8b_to_8b_copy_array(sk + SEED_BYTES, sigma, VEC_K_SIZE_BYTES);
  for (size_t i = 0; i < PUBLIC_KEY_BYTES; i++) {
    sk[(SEED_BYTES + VEC_K_SIZE_BYTES) + i][0] = pk[i];
    for (size_t d = 1; d < N_SHARES; d++) {
      sk[(SEED_BYTES + VEC_K_SIZE_BYTES) + i][d] = 0;
    }
  }
}

void MSK_hqc_secret_key_from_string(MSK_uint64_t y[VEC_N1_SIZE_64],
                                    MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                                    const MSK_uint8_t sk[SECRET_KEY_BYTES]) {
  MSK_seedexpander_ctx sk_seedexpander;
  MSK_uint8_t sk_seed[SEED_BYTES] = {{0}};

  MSK_8b_to_8b_copy_array(sk_seed, sk, SEED_BYTES);
  MSK_8b_to_8b_copy_array(sigma, sk + SEED_BYTES, VEC_K_SIZE_BYTES);
  MSK_seedexpander_init(&sk_seedexpander, sk_seed, SEED_BYTES);

  MSK_vect_set_random_fixed_weight(y, &sk_seedexpander, PARAM_OMEGA);
}

void hqc_public_key_to_string(uint8_t pk[PUBLIC_KEY_BYTES],
                              const uint8_t pk_seed[SEED_BYTES],
                              const uint64_t s[VEC_N_SIZE_64]) {
  memcpy(pk, pk_seed, SEED_BYTES);
  memcpy(pk + SEED_BYTES, s, VEC_N_SIZE_BYTES);
}

void MSK_hqc_public_key_from_string(MSK_uint64_t h[VEC_N_SIZE_64],
                                    MSK_uint64_t s[VEC_N_SIZE_64],
                                    const uint8_t pk[PUBLIC_KEY_BYTES]) {
  seedexpander_ctx pk_seedexpander;
  uint8_t pk_seed[SEED_BYTES] = {0};
  uint64_t tmph[VEC_N_SIZE_64] = {0};
  uint64_t tmps[VEC_N_SIZE_64] = {0};

  memcpy(pk_seed, pk, SEED_BYTES);

  seedexpander_init(&pk_seedexpander, pk_seed, SEED_BYTES);
  vect_set_random(tmph, &pk_seedexpander);

  memcpy(tmps, pk + SEED_BYTES, VEC_N_SIZE_BYTES);

  for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
    h[i][0] = tmph[i];
    s[i][0] = tmps[i];
  }
}

void MSK_hqc_ciphertext_to_string(uint8_t ct[CIPHERTEXT_BYTES],
                                  const MSK_uint64_t mu[VEC_N_SIZE_64],
                                  const MSK_uint64_t mv[VEC_N1N2_SIZE_64],
                                  const MSK_uint8_t msalt[SALT_SIZE_64]) {
  uint64_t u[VEC_N_SIZE_64] = {0};
  uint64_t v[VEC_N1N2_SIZE_64] = {0};
  uint8_t salt[SALT_SIZE_BYTES] = {0};

  // unmasked salt, u and v
  for (size_t i = 0; i < SALT_SIZE_BYTES; i++) {
    salt[i] = MSK_8b_unmask(msalt[i]);
  }

  for (size_t i = 0; i < VEC_N1N2_SIZE_64; i++) {
    v[i] = MSK_64b_unmask(mv[i]);
  }
  for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
    u[i] = MSK_64b_unmask(mu[i]);
  }

  memcpy(ct, u, VEC_N_SIZE_BYTES);
  memcpy(ct + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
  memcpy(ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, salt, SALT_SIZE_BYTES);
}

/**
 * @brief Parse a ciphertext from a string
 *
 * The ciphertext is composed of vectors <b>u</b>, <b>v</b> and salt.
 *
 * @param[out] u uint64_t representation of vector u
 * @param[out] v uint64_t representation of vector v
 * @param[out] d String containing the hash d
 * @param[in] ct String containing the ciphertext
 */
void hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint8_t *salt,
                                const uint8_t *const ct) {
  load8_arr(u, VEC_N_SIZE_64, ct, VEC_N_SIZE_BYTES);
  load8_arr(v, VEC_N1N2_SIZE_64, ct + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES);
  memcpy(salt, ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, SALT_SIZE_BYTES);
}

void MSK_hqc_ciphertext_from_string(MSK_uint64_t u[VEC_N_SIZE_64],
                                    MSK_uint64_t v[VEC_N1N2_SIZE_64],
                                    MSK_uint64_t salt[SALT_SIZE_64],
                                    const uint8_t ct[CIPHERTEXT_BYTES]) {
  uint64_t tmpu[VEC_N_SIZE_64] = {0};
  uint64_t tmpv[VEC_N1N2_SIZE_64] = {0};
  uint64_t tmpsalt[SALT_SIZE_64] = {0};

  memcpy(tmpu, ct, VEC_N_SIZE_BYTES);
  memcpy(tmpv, ct + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES);
  memcpy(tmpsalt, ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, SALT_SIZE_BYTES);

  for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
    u[i][0] = tmpu[i];
  }
  // copy of the last byte of u

  // offset = VEC_N_SIZE_BYTES;
  for (size_t i = 0; i < VEC_N1N2_SIZE_64; i++) {
    v[i][0] = tmpv[i];
  }
  // offset+=VEC_N1N2_SIZE_BYTES;
  for (size_t i = 0; i < SALT_SIZE_64; i++) {
    salt[i][0] = tmpsalt[i];
  }
}

void MSK_store64_to_8(uint8_t *dest, const MSK_uint64_t src[],
                      const size_t size64) {
  for (size_t i = 0; i < size64; i++) {
    for (size_t j = 0; j < 8; j++) {
      for (size_t d = 0; d < N_SHARES; d++) {
        dest[i * 8 * (N_SHARES) + j * (N_SHARES) + d] =
            (src[i][d] >> (8 * j)) & 0xFF;
      }
    }
  }
}
