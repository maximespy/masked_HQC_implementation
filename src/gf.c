/**
 * @file gf.c
 * @brief Computations in Galois field GF2^8 (GF for short) with primitive
 * polynomial 1 + x^2 + x^3 + x^4 + x^8
 */

#include <string.h>

#include "gf.h"
#include "parameters.h"

//------------------------ constants--------------------------

const uint16_t gf_exp[258] = {
    1,   2,   4,   8,   16,  32,  64,  128, 29,  58,  116, 232, 205, 135, 19,
    38,  76,  152, 45,  90,  180, 117, 234, 201, 143, 3,   6,   12,  24,  48,
    96,  192, 157, 39,  78,  156, 37,  74,  148, 53,  106, 212, 181, 119, 238,
    193, 159, 35,  70,  140, 5,   10,  20,  40,  80,  160, 93,  186, 105, 210,
    185, 111, 222, 161, 95,  190, 97,  194, 153, 47,  94,  188, 101, 202, 137,
    15,  30,  60,  120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225,
    223, 163, 91,  182, 113, 226, 217, 175, 67,  134, 17,  34,  68,  136, 13,
    26,  52,  104, 208, 189, 103, 206, 129, 31,  62,  124, 248, 237, 199, 147,
    59,  118, 236, 197, 151, 51,  102, 204, 133, 23,  46,  92,  184, 109, 218,
    169, 79,  158, 33,  66,  132, 21,  42,  84,  168, 77,  154, 41,  82,  164,
    85,  170, 73,  146, 57,  114, 228, 213, 183, 115, 230, 209, 191, 99,  198,
    145, 63,  126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75,
    150, 49,  98,  196, 149, 55,  110, 220, 165, 87,  174, 65,  130, 25,  50,
    100, 200, 141, 7,   14,  28,  56,  112, 224, 221, 167, 83,  166, 81,  162,
    89,  178, 121, 242, 249, 239, 195, 155, 43,  86,  172, 69,  138, 9,   18,
    36,  72,  144, 61,  122, 244, 245, 247, 243, 251, 235, 203, 139, 11,  22,
    44,  88,  176, 125, 250, 233, 207, 131, 27,  54,  108, 216, 173, 71,  142,
    1,   2,   4};

const uint16_t gf_log[256] = {
    0,   0,   1,   25,  2,   50,  26,  198, 3,   223, 51,  238, 27,  104, 199,
    75,  4,   100, 224, 14,  52,  141, 239, 129, 28,  193, 105, 248, 200, 8,
    76,  113, 5,   138, 101, 47,  225, 36,  15,  33,  53,  147, 142, 218, 240,
    18,  130, 69,  29,  181, 194, 125, 106, 39,  249, 185, 201, 154, 9,   120,
    77,  228, 114, 166, 6,   191, 139, 98,  102, 221, 48,  253, 226, 152, 37,
    179, 16,  145, 34,  136, 54,  208, 148, 206, 143, 150, 219, 189, 241, 210,
    19,  92,  131, 56,  70,  64,  30,  66,  182, 163, 195, 72,  126, 110, 107,
    58,  40,  84,  250, 133, 186, 61,  202, 94,  155, 159, 10,  21,  121, 43,
    78,  212, 229, 172, 115, 243, 167, 87,  7,   112, 192, 247, 140, 128, 99,
    13,  103, 74,  222, 237, 49,  197, 254, 24,  227, 165, 153, 119, 38,  184,
    180, 124, 17,  68,  146, 217, 35,  32,  137, 46,  55,  63,  209, 91,  149,
    188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97,  242, 86,  211, 171,
    20,  42,  93,  158, 132, 60,  57,  83,  71,  109, 65,  162, 31,  45,  67,
    216, 183, 123, 164, 118, 196, 23,  73,  236, 127, 12,  111, 246, 108, 161,
    59,  82,  41,  157, 85,  170, 251, 96,  134, 177, 187, 204, 62,  90,  203,
    89,  95,  176, 156, 169, 160, 81,  11,  245, 22,  235, 122, 117, 44,  215,
    79,  174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80,  88,
    175};

//------------------------ tools --------------------------

/**
 * @brief Compute number of trailing zero bits.
 *
 * @return Rumber of trailing zero bits in a.
 * @param[in] a Operand
 */
uint16_t trailing_zero_bits_count(const uint16_t a) {
  uint16_t res = 0;
  uint16_t tmp;
  uint16_t mask = 0xFFFF;
  for (size_t i = 0; i < 14; i++) {
    tmp = 1 - ((a >> i) & 0x0001);
    res += tmp & mask;
    mask &= -tmp;
  }
  return res;
}

//------------------------ masked computations

/**
 * Carryless multiplication of two boolean-masked polynomials a and b.
 *
 * @param[out] [x] Product result x = a * b, array of two N_SHARES-shares
 * boolean-masked bytes
 * @param[inout] [a] First boolean-masked polynomial operand, N_SHARES-shares
 * boolean-masked byte (refreshed)
 * @param[inout] [b] Second boolean-masked polynomial operand, N_SHARES-shares
 * boolean-masked byte (refreshed)
 */
void MSK_caryless_mul(MSK_uint8_t x[2], MSK_uint8_t a, MSK_uint8_t b) {
  MSK_SNI_8b_inplace_refresh(a);
  MSK_SNI_8b_inplace_refresh(b);
  memset(x, 0, N_SHARES * 2);
  for (size_t i = 0; i < 8; i++) {
    MSK_uint8_t t;

    for (size_t d = 0; d < N_SHARES; d++) {
      uint8_t temp = ((a[d]) >> i) & 1;
      temp *= (uint8_t)(-1);
      t[d] = temp;
    }
    MSK_uint8_t u;
    MSK_SNI_8b_and(u, t, b);

    if (i > 0) {
      for (size_t d = 0; d < N_SHARES; d++) {
        x[1][d] ^= (u[d] >> (8 - i));
      }
      MSK_SNI_8b_inplace_refresh(u);
      for (size_t d = 0; d < N_SHARES; d++) {
        x[0][d] ^= (u[d] & (((uint8_t)(1) << (8 - i)) - 1)) << i;
      }
    } else {
      MSK_8b_to_8b_copy(x[0], u);
      MSK_SNI_8b_inplace_refresh(x[0]);
    }
  }
}

/**
 * @brief Reduce boolean-masked polynomial a modulo primitive polynomial
 * GF_POLY.
 *
 * Note: loops are interchanged to move shares manipulation away from each
 * other
 *
 * @param[out] [x] Reduced polynomial (x = a mod GF_POLY), 16-bits
 * N_SHARES-shares boolean-masked value
 * @param[inout] [a] Input polynomial of degree less than 64, 64-bits
 * N_SHARES-shares boolean-masked value (refreshed)
 * @param[in] deg Degree of polynomial a
 */
void MSK_gf_reduce(MSK_uint16_t x, MSK_uint64_t a, const size_t deg) {
  uint16_t z1, z2, rmdr, dist;
  MSK_uint64_t mod;

  // Deduce the number of steps of reduction
  size_t steps = CEIL_DIVIDE(deg - (PARAM_M - 1), PARAM_GF_POLY_M2);

  // Reduce
  for (size_t i = 0; i < steps; i++) {
    MSK_SNI_64b_inplace_refresh(a); // no path from input to output
    for (size_t d = 0; d < N_SHARES; d++) {
      mod[d] = a[d] >> PARAM_M;
      a[d] &= (1 << PARAM_M) - 1;
      a[d] ^= mod[d];
      z1 = 0;
      rmdr = PARAM_GF_POLY ^ 1;

      for (size_t j = PARAM_GF_POLY_WT - 2; j; --j) {
        z2 = trailing_zero_bits_count(rmdr);
        dist = z2 - z1;
        mod[d] <<= dist;
        a[d] ^= mod[d];
        rmdr ^= 1 << z2;
        z1 = z2;
      }
      x[d] = a[d] & 0xFFFF;
    }
    MSK_SNI_16b_inplace_refresh(x);
    // MSK_64b_refresh(mod);
  }
}

/**
 * @brief Square boolean-masked element of GF
 *
 * @param[out] [x] Square of a (x = a^2), 16-bits N_SHARES-shares boolean-masked
 * value
 * @param[inout] [a] Input element of GF, 16-bits N_SHARES-shares boolean-masked
 * value (refreshed)
 */
void MSK_gf_square(MSK_uint16_t x, MSK_uint16_t a) {
  MSK_uint16_t b;
  MSK_16b_to_16b_copy(b, a);
  MSK_SNI_16b_inplace_refresh(a);
  MSK_gf_mul(x, a, b);
  MSK_SNI_16b_inplace_refresh(x); // no path from input to output
}

void MSK_gf_inverse(MSK_uint16_t x, MSK_uint16_t a) {
  MSK_uint16_t tmp1;
  MSK_uint16_t tmp2;
  MSK_uint16_t inv2;

  MSK_SNI_16b_inplace_refresh(a); // no path from input to output

  MSK_gf_square(inv2, a); // a^2
  MSK_SNI_16b_inplace_refresh(a);
  MSK_gf_mul(tmp1, inv2, a); // a^3
  MSK_SNI_16b_inplace_refresh(inv2);
  MSK_gf_square(x, inv2);    // a^4
  MSK_gf_mul(tmp2, x, tmp1); // a^7
  MSK_SNI_16b_inplace_refresh(x);
  MSK_gf_mul(tmp1, x, tmp2); // a^11
  MSK_SNI_16b_inplace_refresh(x);
  MSK_gf_mul(inv2, tmp1, x); // a^15
  MSK_gf_square(x, inv2);    // a^30
  MSK_gf_square(inv2, x);    // a^60
  MSK_gf_square(x, inv2);    // a^120
  MSK_SNI_16b_inplace_refresh(tmp2);
  MSK_gf_mul(inv2, x, tmp2); // a^127
  MSK_gf_square(x, inv2);    // a^254
}

void MSK_gf_mul(MSK_uint16_t x, const MSK_uint16_t a, const MSK_uint16_t b) {
  MSK_uint8_t c[2] = {0};
  MSK_uint8_t a_8, b_8;

  MSK_16b_to_8b_copy(a_8, a);
  MSK_16b_to_8b_copy(b_8, b);

  MSK_caryless_mul(c, a_8, b_8);
  MSK_uint64_t tmp = {0};

  for (size_t d = 0; d < N_SHARES; d++) {
    tmp[d] = c[0][d] ^ ((uint16_t)c[1][d] << 8);
  }
  MSK_gf_reduce(x, tmp, 2 * (PARAM_M - 1));
}

void MSK_semi_gf_mul(MSK_uint16_t x, MSK_uint16_t a, const uint16_t b) {
  MSK_SNI_16b_inplace_refresh(a);
  for (size_t i = 0; i < N_SHARES; i++) {
    x[i] = gf_mul(a[i], b);
  }
}

void MSK_8b_semi_gf_mul(MSK_uint8_t x, MSK_uint8_t a, const uint8_t b) {
  MSK_SNI_8b_inplace_refresh(a);
  for (size_t i = 0; i < N_SHARES; i++) {
    x[i] = (uint8_t)gf_mul(a[i], b);
  }
}

//------------------------ unmasked operations

uint16_t gf_reduce(uint64_t a, const uint8_t deg) {
  uint16_t z1, z2, rmdr, dist;
  uint64_t mod;

  // Deduce the number of steps of reduction
  size_t steps = CEIL_DIVIDE(deg - (PARAM_M - 1), PARAM_GF_POLY_M2);

  // Reduce
  for (size_t i = 0; i < steps; i++) {
    mod = a >> PARAM_M;
    a &= (1 << PARAM_M) - 1;
    a ^= mod;

    z1 = 0;
    rmdr = PARAM_GF_POLY ^ 1;
    for (size_t j = PARAM_GF_POLY_WT - 2; j; --j) {
      z2 = trailing_zero_bits_count(rmdr);
      dist = z2 - z1;
      mod <<= dist;
      a ^= mod;
      rmdr ^= 1 << z2;
      z1 = z2;
    }
  }

  return (uint16_t)a;
}

/**
 * Carryless multiplication of two polynomials a and b.
 *
 * Implementation of the algorithm mul1 in
 * https://hal.inria.fr/inria-00188261v4/document with s = 2 and w = 8
 *
 * @param[out] Polynomial x = a * b
 * @param[in] a First polynomial
 * @param[in] b Second polynomial
 */
void gf_carryless_mul(uint8_t x[2], const uint8_t a, const uint8_t b) {
  uint16_t h = 0, l = 0, g = 0, u[4];
  uint32_t tmp1, tmp2;
  uint16_t mask;
  u[0] = 0;
  u[1] = b & 0x7F;
  u[2] = u[1] << 1;
  u[3] = u[2] ^ u[1];
  tmp1 = a & 3;

  for (size_t i = 0; i < 4; i++) {
    tmp2 = (uint32_t)(tmp1 - i);
    g ^= (u[i] & (uint32_t)(0 - (1 - ((uint32_t)(tmp2 | (0 - tmp2)) >> 31))));
  }

  l = g;
  h = 0;

  for (size_t i = 2; i < 8; i += 2) {
    g = 0;
    tmp1 = (a >> i) & 3;
    for (size_t j = 0; j < 4; j++) {
      tmp2 = (uint32_t)(tmp1 - j);
      g ^= (u[j] & (uint32_t)(0 - (1 - ((uint32_t)(tmp2 | (0 - tmp2)) >> 31))));
    }

    l ^= g << i;
    h ^= g >> (8 - i);
  }

  mask = (-((b >> 7) & 1));
  l ^= ((a << 7) & mask);
  h ^= ((a >> 1) & mask);

  x[0] = (uint8_t)l;
  x[1] = (uint8_t)h;
}

uint16_t gf_mul(const uint16_t a, const uint16_t b) {
  uint8_t c[2] = {0};
  gf_carryless_mul(c, (uint8_t)a, (uint8_t)b);
  uint16_t tmp = c[0] ^ (c[1] << 8);
  return gf_reduce(tmp, 2 * (PARAM_M - 1));
}

uint16_t gf_square(const uint16_t a) {
  uint32_t b = a;
  uint32_t s = b & 1;

  for (size_t i = 1; i < PARAM_M; i++) {
    b <<= 1;
    s ^= b & (1 << 2 * i);
  }

  return gf_reduce(s, 2 * (PARAM_M - 1));
}

uint16_t gf_inverse(const uint16_t a) {
  uint16_t inv = a;
  uint16_t tmp1, tmp2;

  inv = gf_square(a);       /* a^2 */
  tmp1 = gf_mul(inv, a);    /* a^3 */
  inv = gf_square(inv);     /* a^4 */
  tmp2 = gf_mul(inv, tmp1); /* a^7 */
  tmp1 = gf_mul(inv, tmp2); /* a^11 */
  inv = gf_mul(tmp1, inv);  /* a^15 */
  inv = gf_square(inv);     /* a^30 */
  inv = gf_square(inv);     /* a^60 */
  inv = gf_square(inv);     /* a^120 */
  inv = gf_mul(inv, tmp2);  /* a^127 */
  inv = gf_square(inv);     /* a^254 */
  return inv;
}
