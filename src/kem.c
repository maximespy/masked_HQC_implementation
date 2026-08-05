/**
 * @file kem.c
 * @brief Implementation of api.h
 */

#include <string.h>

#include "domains.h"
#include "hqc.h"
#include "kem.h"
#include "parsing.h"
#include "shake_ds.h"
#include "vector.h"

// LOG macro
#if VERBOSITY_LEVEL != 0
#define LOG(fp, format, ...)                                                   \
  do {                                                                         \
    fprintf((fp), (format)__VA_OPT__(, ) __VA_ARGS__);                         \
  } while (0)
#else
#define LOG(fp, format, ...)                                                   \
  do {                                                                         \
  } while (0)
#endif

int MSK_crypto_kem_keypair(uint8_t pk[PUBLIC_KEY_BYTES],
                           MSK_uint8_t sk[SECRET_KEY_BYTES]) {
  LOG(stdout, "\n\n\n\n### KEYGEN ###");
  MSK_hqc_pke_keygen(pk, sk);

  return 0;
}

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
  MSK_uint8_t masked_sk[SECRET_KEY_BYTES];

  MSK_hqc_pke_keygen(pk, masked_sk);
  MSK_8b_unmask_array(sk, masked_sk, SECRET_KEY_BYTES);

  return 0;
}

int MSK_crypto_kem_enc(uint8_t ct[CIPHERTEXT_BYTES],
                       MSK_uint8_t ss[SHARED_SECRET_BYTES],
                       const uint8_t pk[PUBLIC_KEY_BYTES], int unmasked_salts) {
  LOG(stdout, "\n\n### ENCAPS ###");

  MSK_uint8_t theta[SHAKE256_512_BYTES] = {0};
  MSK_uint64_t MSK_u[VEC_N_SIZE_64] = {0};
  uint64_t u[VEC_N_SIZE_64] = {0};
  MSK_uint64_t MSK_v[VEC_N1N2_SIZE_64] = {0};
  uint64_t v[VEC_N1N2_SIZE_64] = {0};
  MSK_uint8_t tmp[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {
      0};
  MSK_uint8_t *m = tmp;
  MSK_uint8_t *MSK_pk = tmp + VEC_K_SIZE_BYTES;
  uint8_t salt[SALT_SIZE_BYTES] = {0};
  MSK_uint8_t MSK_salt[SALT_SIZE_BYTES] = {0};
  MSK_shake256incctx shake256ctx;

  // refresh MASTER SHAKE STATE so no path from input to output (it's an
  // implicit input)
  MSK_refresh_master_shake_state();
  // Computing m
  MSK_shake_prng(m, VEC_K_SIZE_BYTES);

  // extract 32 bytes of pk to masked format
  MSK_8b_mask_array(MSK_pk, pk, SALT_SIZE_BYTES * 2);

  // salt is a public value but we must keep the random numbers generator
  // synchronized for KATs, so for KATs with ciphertext checking salt is
  // generated from the masked PRNG
  if (unmasked_salts) {
    shake_prng(salt, SALT_SIZE_BYTES);
    MSK_8b_mask_array(MSK_salt, salt, SALT_SIZE_BYTES);
  } else {
    MSK_shake_prng(MSK_salt, SALT_SIZE_BYTES);
  }

  // Computing theta
  MSK_8b_to_8b_copy_array(tmp + VEC_K_SIZE_BYTES + SALT_SIZE_BYTES * 2,
                          MSK_salt, SALT_SIZE_BYTES);
  MSK_shake256_512_ds(
      theta, &shake256ctx, tmp,
      VEC_K_SIZE_BYTES + (SALT_SIZE_BYTES * 2) + SALT_SIZE_BYTES, G_FCT_DOMAIN);

  // Encrypting m
  MSK_SNI_8b_inplace_refresh_array(m, VEC_K_SIZE_BYTES);
  MSK_hqc_pke_encrypt(MSK_u, MSK_v, m, theta, pk);

  // Computing shared secret
  // u and v are public values
  MSK_store64_to_8((uint8_t *)tmp + VEC_K_SIZE_BYTES * (N_SHARES), MSK_u,
                   VEC_N_SIZE_64);
  MSK_store64_to_8((uint8_t *)tmp +
                       (VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES) * (N_SHARES),
                   MSK_v, VEC_N1N2_SIZE_64);
  MSK_shake256_512_ds(ss, &shake256ctx, tmp,
                      VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES,
                      K_FCT_DOMAIN);

  // Unmask and assemble ciphertext
  MSK_64b_unmask_array(u, MSK_u, VEC_N_SIZE_64);
  MSK_64b_unmask_array(v, MSK_v, VEC_N1N2_SIZE_64);
  if (!unmasked_salts) {
    MSK_8b_unmask_array(salt, MSK_salt, SALT_SIZE_BYTES);
  }
  memcpy(ct, u, VEC_N_SIZE_BYTES);
  memcpy(ct + VEC_N_SIZE_BYTES, v, VEC_N1N2_SIZE_BYTES);
  memcpy(ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, salt, SALT_SIZE_BYTES);

#if VERBOSITY_LEVEL != 0
  fprintf_uint8_t_array(stdout, "\npk: ", pk, PUBLIC_KEY_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nm: ", m, VEC_K_SIZE_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\ntheta: ", theta, SHAKE256_512_BYTES, "\n");
  fprintf_uint8_t_array(stdout, "\nciphertext: ", ct, CIPHERTEXT_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nsecret 1: ", ss, SHARED_SECRET_BYTES, "\n\n");
#endif

  return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss,
                   const unsigned char *pk) {
  MSK_uint8_t masked_ss[SHARED_SECRET_BYTES];

  MSK_crypto_kem_enc(ct, masked_ss, pk, 0);
  MSK_8b_unmask_array(ss, masked_ss, SHARED_SECRET_BYTES);

  return 0;
}

void MSK_crypto_kem_dec(MSK_uint8_t ss[SHARED_SECRET_BYTES], MSK_uint8_t result,
                        const uint8_t ct[CIPHERTEXT_BYTES],
                        MSK_uint8_t sk[SECRET_KEY_BYTES]) {
  LOG(stdout, "\n\n### DECAPS ###");

  MSK_uint64_t res64 = {0};
  uint64_t u[VEC_N_SIZE_64] = {0};
  uint64_t v[VEC_N1N2_SIZE_64] = {0};
  MSK_uint64_t MSK_u[VEC_N_SIZE_64] = {0};
  MSK_uint64_t MSK_v[VEC_N1N2_SIZE_64] = {0};
  MSK_uint8_t m[VEC_K_SIZE_BYTES] = {0};
  MSK_uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
  MSK_uint8_t theta[SHAKE256_512_BYTES] = {0};
  MSK_uint64_t u2[VEC_N_SIZE_64] = {0};
  MSK_uint64_t v2[VEC_N1N2_SIZE_64] = {0};
  MSK_uint8_t tmp[VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES] = {
      0};
  MSK_uint64_t MSK_salt[SALT_SIZE_64] = {0};
  uint64_t salt[SALT_SIZE_64] = {0};
  MSK_shake256incctx shake256ctx;
  uint8_t pk[PUBLIC_KEY_BYTES] = {0};
  MSK_uint8_t *MSK_pk = tmp + VEC_K_SIZE_BYTES;

  // Retrieving u, v and salt from ciphertext
  memcpy(u, ct, VEC_N_SIZE_BYTES);
  MSK_64b_mask_array(MSK_u, u, VEC_N_SIZE_64);
  memcpy(v, ct + VEC_N_SIZE_BYTES, VEC_N1N2_SIZE_BYTES);
  MSK_64b_mask_array(MSK_v, v, VEC_N1N2_SIZE_64);
  memcpy(salt, ct + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES, SALT_SIZE_BYTES);
  MSK_64b_mask_array(MSK_salt, salt, SALT_SIZE_BYTES);
  // ct is a public input, no refresh

  // Decrypting
  MSK_hqc_pke_decrypt(m, sigma, MSK_u, MSK_v, sk); // decrypt is MIMO-SNI

  for (size_t i = 0; i < PUBLIC_KEY_BYTES; i++) {
    pk[i] = sk[SEED_BYTES + VEC_K_SIZE_BYTES + i][0];
  }

  // extract 32 bytes of pk to masked format
  MSK_8b_mask_array(MSK_pk, pk, SALT_SIZE_BYTES * 2);

  // Computing theta
  // pk and salt are public values, no sharing necessary

  MSK_8b_to_8b_copy_array(tmp, m, VEC_K_SIZE_BYTES);
  MSK_store64_to_8((uint8_t *)tmp +
                       (VEC_K_SIZE_BYTES + SALT_SIZE_BYTES * 2) * (N_SHARES),
                   MSK_salt, SALT_SIZE_64);
  MSK_shake256_512_ds(
      theta, &shake256ctx, tmp,
      VEC_K_SIZE_BYTES + (SALT_SIZE_BYTES * 2) + SALT_SIZE_BYTES, G_FCT_DOMAIN);

#if VERBOSITY_LEVEL != 0
  MSK_8b_fprintf_array(stdout, "\n\ntheta: ", theta, SHAKE256_512_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\ntmp: ", tmp,
                       VEC_K_SIZE_BYTES + 3 * SALT_SIZE_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nm: ", m, VEC_K_SIZE_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\npke: ", MSK_pk, 2 * SALT_SIZE_BYTES, "\n");
  MSK_64b_fprintf_array(stdout, "\nsalt: ", MSK_salt, SALT_SIZE_64, "\n");
#endif

  // Encrypting m'
  MSK_hqc_pke_encrypt(u2, v2, m, theta, pk); // encrypt is MIMO-SNI

  // Check if c != c'. MSK_vect_compare is MIMO-SNI.
  // res64 |= 0 if MSK_u==u2, else res64 |= 1
  MSK_vect_compare(res64, MSK_u, u2, VEC_N_SIZE_64);
  // res64 |= 0 if MSK_u==u2, else res64 |= 1
  MSK_vect_compare(res64, MSK_v, v2, VEC_N1N2_SIZE_64);

  MSK_64b_to_8b_copy(result, res64);

  for (size_t i = 0; i < VEC_K_SIZE_BYTES; ++i) {
    MSK_8b_if(tmp[i], m[i], sigma[i], result);
    MSK_SNI_8b_inplace_refresh(result);
  }

  // Computing shared secret
  // u and v are public values
  MSK_store64_to_8((uint8_t *)tmp + VEC_K_SIZE_BYTES * (N_SHARES), MSK_u,
                   VEC_N_SIZE_64);
  MSK_store64_to_8((uint8_t *)tmp +
                       (VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES) * (N_SHARES),
                   MSK_v, VEC_N1N2_SIZE_64);
  MSK_shake256_512_ds(ss, &shake256ctx, tmp,
                      VEC_K_SIZE_BYTES + VEC_N_SIZE_BYTES + VEC_N1N2_SIZE_BYTES,
                      K_FCT_DOMAIN);

#if VERBOSITY_LEVEL != 0
  fprintf_uint8_t_array(stdout, "\n\npk: ", pk, PUBLIC_KEY_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nsk: ", sk, SEED_BYTES + VEC_K_SIZE_BYTES, "");
  fprintf_uint8_t_array(stdout, "", pk, PUBLIC_KEY_BYTES, "\n");

  fprintf_uint8_t_array(stdout, "\n\nciphertext: ", ct, CIPHERTEXT_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nm: ", m, VEC_K_SIZE_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\ntheta: ", theta, SHAKE256_512_BYTES, "\n");
  printf("\n\n# Checking Ciphertext- Begin #\n");
  MSK_64b_fprintf_array(stdout, "\nu2: ", u2, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nv2: ", v2, VEC_N1N2_SIZE_64, "\n");
  printf("\n# Checking Ciphertext - End #\n");
#endif
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct,
                   const unsigned char *sk) {
  MSK_uint8_t masked_ss[SHARED_SECRET_BYTES];
  MSK_uint8_t masked_result;
  MSK_uint8_t masked_sk[SECRET_KEY_BYTES];

  MSK_8b_mask_array(masked_sk, sk, SECRET_KEY_BYTES);
  MSK_crypto_kem_dec(masked_ss, masked_result, ct, masked_sk);
  MSK_8b_unmask_array(ss, masked_ss, SHARED_SECRET_BYTES);

  return MSK_8b_unmask(masked_result);
}
