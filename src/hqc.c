/**
 * @file hqc.c
 * @brief Implementation of hqc.h
 */

#include "hqc.h"
#include "code.h"
#include "gf2x.h"
#include "parsing.h"
#include "vector.h"

void MSK_hqc_pke_keygen(uint8_t pk[PUBLIC_KEY_BYTES],
                        MSK_uint8_t sk[SECRET_KEY_BYTES]) {
  MSK_seedexpander_ctx sk_seedexpander;
  seedexpander_ctx pk_seedexpander;
  MSK_uint8_t sk_seed[SEED_BYTES] = {0};
  MSK_uint8_t sigma[VEC_K_SIZE_BYTES] = {0};
  MSK_uint8_t tmpk_seed[SEED_BYTES] = {0};
  uint8_t pk_seed[SEED_BYTES] = {0};
  MSK_uint64_t x[VEC_N_SIZE_64] = {0};
  MSK_uint64_t y[VEC_N_SIZE_64] = {0};
  uint64_t h[VEC_N_SIZE_64] = {0};
  uint64_t s[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp_h[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp_s[VEC_N_SIZE_64] = {0};

  // TO GAIN MEMORY, LATER
  // we can fuse x and y, and tmp_s and h
  // gen y, gen h in tmp_s
  // vectmul(tmp_s,y,tmp_s)
  // gen x in y
  // vectadd(tmps,y)

  // refresh MASTER SHAKE STATE so no path from input to output (it's an
  // implicit input)
  MSK_refresh_master_shake_state();

  // Create seed_expanders for public key and secret key
  MSK_shake_prng(sk_seed, SEED_BYTES);
  MSK_shake_prng(sigma, VEC_K_SIZE_BYTES);

  MSK_seedexpander_init(&sk_seedexpander, sk_seed, SEED_BYTES);

  // Masked operation but we'll unmask the output since it's the seed for the
  // public key
  MSK_shake_prng(tmpk_seed, SEED_BYTES);
  for (uint8_t i = 0; i < SEED_BYTES; i++) {
    pk_seed[i] = MSK_8b_unmask(tmpk_seed[i]);
  }

  seedexpander_init(&pk_seedexpander, pk_seed, SEED_BYTES);

  // Compute secret key
  MSK_vect_set_random_fixed_weight(y, &sk_seedexpander, PARAM_OMEGA);
  MSK_vect_set_random_fixed_weight(x, &sk_seedexpander, PARAM_OMEGA);

  // Compute public key
  vect_set_random(h, &pk_seedexpander);

  for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
    tmp_h[i][0] = h[i];
  }

  MSK_vect_mul(tmp_s, y, tmp_h);
  MSK_vect_add(tmp_s, x, tmp_s, VEC_N_SIZE_64);

  for (size_t i = 0; i < VEC_N_SIZE_64; i++) {
    s[i] = MSK_64b_unmask(tmp_s[i]);
  }

  // Parse keys to string
  hqc_public_key_to_string(pk, pk_seed, s);
  MSK_SNI_8b_inplace_refresh_array(sigma, VEC_K_SIZE_BYTES);
  MSK_hqc_secret_key_to_string(sk, sk_seed, sigma, pk);

#if VERBOSITY_LEVEL != 0
  MSK_8b_fprintf_array(stdout, "\n\nsk_seed: ", sk_seed, SEED_BYTES, "\n");
  MSK_8b_fprintf_array(stdout, "\nsigma: ", sigma, VEC_K_SIZE_BYTES, "\n");
  MSK_64b_fprintf_array(stdout, "\nx: ", x, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\ny: ", y, VEC_N_SIZE_64, "\n");
  fprintf_uint8_t_array(stdout, "\npk_seed: ", pk_seed, SEED_BYTES, "\n");
  fprintf_uint64_t_array(stdout, "\nh: ", h, VEC_N_SIZE_64, "\n");
  fprintf_uint64_t_array(stdout, "\ns: ", s, VEC_N_SIZE_64, "\n");
  MSK_8b_fprintf_array(stdout, "\nsk: ", sk,
                       SEED_BYTES + VEC_K_SIZE_BYTES + PUBLIC_KEY_BYTES, "\n");
  fprintf_uint8_t_array(stdout, "\npk: ", pk, PUBLIC_KEY_BYTES, "\n\n");
#endif
}

void MSK_hqc_pke_encrypt(MSK_uint64_t u[VEC_N_SIZE_64],
                         MSK_uint64_t v[VEC_N1N2_SIZE_64],
                         MSK_uint8_t m[VEC_K_SIZE_BYTES],
                         MSK_uint8_t theta[SHAKE256_512_BYTES],
                         const uint8_t pk[PUBLIC_KEY_BYTES]) {
  MSK_seedexpander_ctx vec_seedexpander;
  MSK_uint64_t h[VEC_N_SIZE_64] = {0};
  MSK_uint64_t s[VEC_N_SIZE_64] = {0};
  MSK_uint64_t r1[VEC_N_SIZE_64] = {0};
  MSK_uint64_t r2[VEC_N_SIZE_64] = {0};
  MSK_uint64_t e[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp1[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp2[VEC_N_SIZE_64] = {0};

  // Create seed_expander from theta
  MSK_seedexpander_init(&vec_seedexpander, theta, SEED_BYTES);

  // Retrieve h and s from public key
  MSK_hqc_public_key_from_string(h, s, pk);

  // Generate r1, r2 and e
  MSK_vect_set_random_fixed_weight(r2, &vec_seedexpander, PARAM_OMEGA_R);
  MSK_vect_set_random_fixed_weight(e, &vec_seedexpander, PARAM_OMEGA_E);
  MSK_vect_set_random_fixed_weight(r1, &vec_seedexpander, PARAM_OMEGA_R);

  // Compute u = r1 + r2.h
  MSK_vect_mul(u, r2, h);
  MSK_vect_add(u, r1, u, VEC_N_SIZE_64);

  // Compute v = m.G by encoding the message
  MSK_code_encode(v, m);
  MSK_vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);

  // Compute v = m.G + s.r2 + e
  MSK_vect_mul(tmp2, r2, s);
  MSK_vect_add(tmp2, e, tmp2, VEC_N_SIZE_64);
  MSK_vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
  MSK_vect_resize(v, PARAM_N1N2, tmp2, PARAM_N);
  // vect_mul, vect_add, vect_resize and encode are MIMO-SNI -> no refresh

#if VERBOSITY_LEVEL != 0
  MSK_64b_fprintf_array(stdout, "\nh: ", h, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\ns: ", s, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nr1: ", r1, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nr2: ", r2, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\ne: ", e, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nnon-truncated v: ", tmp2, VEC_N_SIZE_64,
                        "\n");
  MSK_64b_fprintf_array(stdout, "\nu: ", u, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nv: ", v, VEC_N1N2_SIZE_64, "\n");
#endif
}

uint8_t MSK_hqc_pke_decrypt(MSK_uint8_t m[VEC_K_SIZE_BYTES],
                            MSK_uint8_t sigma[VEC_K_SIZE_BYTES],
                            MSK_uint64_t u[VEC_N_SIZE_64],
                            const MSK_uint64_t v[VEC_N1N2_SIZE_64],
                            MSK_uint8_t sk[SECRET_KEY_BYTES]) {
  MSK_uint64_t y[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp1[VEC_N_SIZE_64] = {0};
  MSK_uint64_t tmp2[VEC_N_SIZE_64] = {0};

  // refresh sk because it's an input
  // only refresh the part that is the sk, not the included pk
  MSK_SNI_8b_inplace_refresh_array(sk, (SEED_BYTES + VEC_K_SIZE_BYTES));

  // Retrieve x, y, pk from secret key
  MSK_hqc_secret_key_from_string(y, sigma, sk);
#ifdef MIMOSNI
  MSK_SNI_8b_inplace_refresh_array(sigma, VEC_K_SIZE_BYTES);
#endif

  // Compute v - u.y
  MSK_vect_resize(tmp1, PARAM_N, v, PARAM_N1N2);
  MSK_vect_mul(tmp2, y, u); // is an input that is public :no refresh
  MSK_vect_add(tmp2, tmp1, tmp2, VEC_N_SIZE_64);
  // vect_add, mult and resize are MIMO-SNI: no refresh needed

#if VERBOSITY_LEVEL != 0
  MSK_64b_fprintf_array(stdout, "\n\nu: ", u, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nv: ", v, VEC_N1N2_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\ny: ", y, VEC_N_SIZE_64, "\n");
  MSK_64b_fprintf_array(stdout, "\nv - u.y: ", tmp2, VEC_N_SIZE_64, "\n");
#endif

  // Compute m by decoding v - u.y
  MSK_code_decode(m, tmp2);

  return 0;
}
