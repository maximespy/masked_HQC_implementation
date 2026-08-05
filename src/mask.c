/**
 * @file mask.c
 * @brief Base operations on masked variables
 */

#include <string.h>

#include "mask.h"
#include "prng.h"

/**********************************************************************
 * Helper macros for bit-width generic definitions                    *
 **********************************************************************/
/**
 * @brief log2(X)-1 with X in {8, 16, 32, 64}
 */
#define LOG2M1(V) (((V) == 8) ? 2 : (((V) == 16) ? 3 : ((((V) == 32) ? 4 : 5))))

/**
 * @brief Left-rotation of a by n positions
 */
#define ROL(a, n) (((a) << (n)) ^ ((a) >> (64 - (n))))

// Instantiate one-parameter macro M with parameter values 8, 16, 32 and 64
#define INSTANTIATE_M8_16_32_64                                                \
  M(8)                                                                         \
  M(16)                                                                        \
  M(32)                                                                        \
  M(64)

// Instantiate two-parameters macro M with parameter values (8, 16), (16, 32),
// and (32, 64)
#define INSTANTIATE_M816_1632_3264                                             \
  M(8, 16)                                                                     \
  M(16, 32)                                                                    \
  M(32, 64)

// Instantiate two-parameters macro M with parameter values (A,B!=A) in
// (8,16,32,64)
#define INSTANTIATE_2DX_M8_16_32_64                                            \
  M(8, 16)                                                                     \
  M(8, 32)                                                                     \
  M(8, 64)                                                                     \
  M(16, 8)                                                                     \
  M(16, 32)                                                                    \
  M(16, 64)                                                                    \
  M(32, 8)                                                                     \
  M(32, 16)                                                                    \
  M(32, 64)                                                                    \
  M(64, 8)                                                                     \
  M(64, 16)                                                                    \
  M(64, 32)

/**********************************************************************
 * Basic functions and gadgets on masked variables                    *
 **********************************************************************/

#define M(BW)                                                                  \
  void MSK_##BW##b_mask(MSK_uint##BW##_t x, const uint##BW##_t a) {            \
    x[0] = (uint##BW##_t)a;                                                    \
    for (size_t i = 1; i < N_SHARES; i++) {                                    \
      x[i] = 0;                                                                \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_mask_array(MSK_uint##BW##_t x[],                            \
                              const uint##BW##_t *const a, const size_t n) {   \
    for (size_t i = 0; i < n; i++) {                                           \
      MSK_##BW##b_mask(x[i], a[i]);                                            \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  uint##BW##_t MSK_##BW##b_unmask(const MSK_uint##BW##_t a) {                  \
    uint##BW##_t x = 0;                                                        \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x ^= a[i];                                                               \
    }                                                                          \
    return x;                                                                  \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_unmask_array(uint##BW##_t *const x,                         \
                                const MSK_uint##BW##_t *const a,               \
                                const size_t n) {                              \
    for (size_t i = 0; i < n; i++) {                                           \
      x[i] = MSK_##BW##b_unmask(a[i]);                                         \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void _MSK_SNI_##BW##b_inplace_refresh(uint##BW##_t *const x,                 \
                                        const size_t n) {                      \
    if (n == 1) {                                                              \
      return;                                                                  \
    }                                                                          \
    if (n == 2) {                                                              \
      uint##BW##_t r = next_##BW();                                            \
      x[0] ^= r;                                                               \
      x[1] ^= r;                                                               \
      return;                                                                  \
    }                                                                          \
    for (size_t i = 0; i < n / 2; i++) {                                       \
      uint##BW##_t r = next_##BW();                                            \
      x[i] ^= r;                                                               \
      x[n / 2 + i] ^= r;                                                       \
    }                                                                          \
    uint##BW##_t *y = x;                                                       \
    uint##BW##_t *z = x + (n / 2);                                             \
                                                                               \
    _MSK_SNI_##BW##b_inplace_refresh(y, n / 2);                                \
    _MSK_SNI_##BW##b_inplace_refresh(z, n - n / 2);                            \
                                                                               \
    for (size_t i = 0; i < n / 2; i++) {                                       \
      uint##BW##_t r = next_##BW();                                            \
      x[i] = r ^ y[i];                                                         \
      x[n / 2 + i] = r ^ z[i];                                                 \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_SNI_##BW##b_inplace_refresh(MSK_uint##BW##_t x) {                   \
    _MSK_SNI_##BW##b_inplace_refresh(x, N_SHARES);                             \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_SNI_##BW##b_inplace_refresh_array(MSK_uint##BW##_t x[],             \
                                             const size_t n) {                 \
    for (size_t i = 0; i < n; i++) {                                           \
      MSK_SNI_##BW##b_inplace_refresh(x[i]);                                   \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void _MSKA_SNI_##BW##b_inplace_refresh(uint##BW##_t *const x,                \
                                         const size_t n) {                     \
    if (n == 1) {                                                              \
      return;                                                                  \
    }                                                                          \
    if (n == 2) {                                                              \
      uint##BW##_t r = next_##BW();                                            \
      x[0] += r;                                                               \
      x[1] -= r;                                                               \
    } else {                                                                   \
      for (size_t i = 0; i < n / 2; i++) {                                     \
        uint##BW##_t r = next_##BW();                                          \
        x[i] += r;                                                             \
        x[n / 2 + i] -= r;                                                     \
      }                                                                        \
      uint##BW##_t *y = x;                                                     \
      uint##BW##_t *z = x + (n / 2);                                           \
                                                                               \
      _MSKA_SNI_##BW##b_inplace_refresh(y, n / 2);                             \
      _MSKA_SNI_##BW##b_inplace_refresh(z, n - n / 2);                         \
                                                                               \
      for (size_t i = 0; i < n / 2; i++) {                                     \
        uint##BW##_t r = next_##BW();                                          \
        x[i] = r + y[i];                                                       \
        x[n / 2 + i] = -r + z[i];                                              \
      }                                                                        \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSKA_SNI_##BW##b_inplace_refresh(MSK_uint##BW##_t x) {                  \
    _MSKA_SNI_##BW##b_inplace_refresh(x, N_SHARES);                            \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_inplace_not(MSK_uint##BW##_t x) { x[0] ^= UINT##BW##_MAX; }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_not(MSK_uint##BW##_t x, const MSK_uint##BW##_t a) {         \
    x[0] = a[0] ^ UINT##BW##_MAX;                                              \
    for (size_t i = 1; i < N_SHARES; i++) {                                    \
      x[i] = a[i];                                                             \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_xor(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,           \
                       const MSK_uint##BW##_t b) {                             \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = a[i] ^ b[i];                                                      \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_xor3(MSK_uint##BW##_t r, const MSK_uint##BW##_t a,          \
                        const MSK_uint##BW##_t b, const MSK_uint##BW##_t c) {  \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      r[i] = a[i] ^ b[i] ^ c[i];                                               \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_xor4(MSK_uint##BW##_t r, const MSK_uint##BW##_t a,          \
                        const MSK_uint##BW##_t b, const MSK_uint##BW##_t c,    \
                        const MSK_uint##BW##_t d) {                            \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      r[i] = a[i] ^ b[i] ^ c[i] ^ d[i];                                        \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_xor5(MSK_uint##BW##_t r, const MSK_uint##BW##_t a,          \
                        const MSK_uint##BW##_t b, const MSK_uint##BW##_t c,    \
                        const MSK_uint##BW##_t d, const MSK_uint##BW##_t e) {  \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      r[i] = a[i] ^ b[i] ^ c[i] ^ d[i] ^ e[i];                                 \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_semi_and(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,      \
                            const uint##BW##_t b) {                            \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = a[i] & b;                                                         \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void _MSK_SNI_##BW##b_and(uint##BW##_t *const x,                             \
                            const uint##BW##_t *const a,                       \
                            const uint##BW##_t *const b, const size_t n) {     \
    for (size_t i = 0; i < n; i++) {                                           \
      x[i] = a[i] & b[i];                                                      \
    }                                                                          \
                                                                               \
    for (size_t i = 0; i < n; i++) {                                           \
      for (size_t j = i + 1; j < n; j++) {                                     \
        uint##BW##_t r = next_##BW();                                          \
        x[i] ^= r;                                                             \
        r ^= a[i] & b[j];                                                      \
        r ^= a[j] & b[i];                                                      \
        x[j] ^= r;                                                             \
      }                                                                        \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_SNI_##BW##b_and(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,       \
                           const MSK_uint##BW##_t b) {                         \
    _MSK_SNI_##BW##b_and(x, a, b, N_SHARES);                                   \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_SNI_##BW##b_or(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,        \
                          const MSK_uint##BW##_t b) {                          \
    MSK_uint##BW##_t t0;                                                       \
    MSK_uint##BW##_t t1;                                                       \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      t0[i] = a[i];                                                            \
      t1[i] = b[i];                                                            \
    }                                                                          \
                                                                               \
    t0[0] = ~t0[0];                                                            \
    t1[0] = ~t1[0];                                                            \
                                                                               \
    MSK_SNI_##BW##b_and(x, t0, t1);                                            \
    x[0] = ~x[0];                                                              \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_right_shift(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,   \
                               const size_t n) {                               \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = (uint##BW##_t)a[i] >> n;                                          \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_left_shift(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,    \
                              const size_t n) {                                \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = (uint##BW##_t)a[i] << n;                                          \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_left_rotate(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,   \
                               const size_t n) {                               \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = ROL(a[i], n);                                                     \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_inplace_left_rotate(MSK_uint##BW##_t x, const size_t n) {   \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = ROL(x[i], n);                                                     \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_semi_add(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,      \
                            const uint##BW##_t b) {                            \
    MSK_uint##BW##_t p;                                                        \
    MSK_uint##BW##_t g;                                                        \
    MSK_uint##BW##_t t0;                                                       \
    MSK_uint##BW##_t t1;                                                       \
    MSK_uint##BW##_t t2;                                                       \
                                                                               \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      p[i] = a[i];                                                             \
      g[i] = a[i] & b;                                                         \
    }                                                                          \
    p[0] ^= b;                                                                 \
                                                                               \
    for (size_t j = 0; j < LOG2M1(BW); j++) {                                  \
      uint8_t pow = 1 << j;                                                    \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        t0[i] = g[i] << pow;                                                   \
        t1[i] = p[i] << pow;                                                   \
      }                                                                        \
                                                                               \
      MSK_uint##BW##_t t2;                                                     \
      MSK_SNI_##BW##b_and(t2, t0, p);                                          \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        t0[i] = t2[i];                                                         \
        g[i] ^= t0[i];                                                         \
      }                                                                        \
                                                                               \
      MSK_SNI_##BW##b_inplace_refresh(t1);                                     \
      MSK_SNI_##BW##b_and(t2, p, t1);                                          \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        p[i] = t2[i];                                                          \
      }                                                                        \
    }                                                                          \
                                                                               \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      t0[i] = g[i] << (1 << LOG2M1(BW));                                       \
    }                                                                          \
                                                                               \
    MSK_SNI_##BW##b_and(t2, t0, p);                                            \
    g[0] ^= t2[0];                                                             \
    x[0] = a[0] ^ b ^ (g[0] << 1);                                             \
                                                                               \
    for (size_t i = 1; i < N_SHARES; i++) {                                    \
      g[i] ^= t2[i];                                                           \
      x[i] = a[i] ^ (g[i] << 1);                                               \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void _MSK_##BW##b_add(uint##BW##_t *const x, const uint##BW##_t *const a,    \
                        const uint##BW##_t *const b, const size_t n) {         \
    uint##BW##_t p[n];                                                         \
    uint##BW##_t g[n];                                                         \
    uint##BW##_t t0[n];                                                        \
    for (size_t i = 0; i < n; i++) {                                           \
      p[i] = a[i] ^ b[i];                                                      \
    }                                                                          \
    _MSK_SNI_##BW##b_and(g, a, b, n);                                          \
                                                                               \
    for (size_t j = 0; j < LOG2M1(BW); j++) {                                  \
      uint8_t pow = 1 << j;                                                    \
      uint##BW##_t t1[n];                                                      \
      for (size_t i = 0; i < n; i++) {                                         \
        t0[i] = g[i] << pow;                                                   \
        t1[i] = p[i] << pow;                                                   \
      }                                                                        \
                                                                               \
      uint##BW##_t t2[n];                                                      \
      _MSK_SNI_##BW##b_and(t2, t0, p, n);                                      \
      for (size_t i = 0; i < n; i++) {                                         \
        t0[i] = t2[i];                                                         \
        g[i] ^= t0[i];                                                         \
      }                                                                        \
                                                                               \
      _MSK_SNI_##BW##b_inplace_refresh(t1, n);                                 \
      _MSK_SNI_##BW##b_and(t2, p, t1, n);                                      \
      for (size_t i = 0; i < n; i++) {                                         \
        p[i] = t2[i];                                                          \
      }                                                                        \
    }                                                                          \
    for (size_t i = 0; i < n; i++) {                                           \
      t0[i] = g[i] << (1 << LOG2M1(BW));                                       \
    }                                                                          \
    uint##BW##_t t2[n];                                                        \
    _MSK_SNI_##BW##b_and(t2, t0, p, n);                                        \
    for (size_t i = 0; i < n; i++) {                                           \
      g[i] ^= t2[i];                                                           \
      x[i] = a[i] ^ b[i] ^ (g[i] << 1);                                        \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_add(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,           \
                       const MSK_uint##BW##_t b) {                             \
    MSK_uint##BW##_t p;                                                        \
    MSK_uint##BW##_t g;                                                        \
    MSK_uint##BW##_t t0;                                                       \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      p[i] = a[i] ^ b[i];                                                      \
    }                                                                          \
    MSK_SNI_##BW##b_and(g, a, b);                                              \
                                                                               \
    for (size_t j = 0; j < LOG2M1(BW); j++) {                                  \
      uint8_t pow = 1 << j;                                                    \
      MSK_uint##BW##_t t1;                                                     \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        t0[i] = g[i] << pow;                                                   \
        t1[i] = p[i] << pow;                                                   \
      }                                                                        \
                                                                               \
      MSK_uint##BW##_t t2;                                                     \
      MSK_SNI_##BW##b_and(t2, t0, p);                                          \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        t0[i] = t2[i];                                                         \
        g[i] ^= t0[i];                                                         \
      }                                                                        \
                                                                               \
      MSK_SNI_##BW##b_inplace_refresh(t1);                                     \
      MSK_SNI_##BW##b_and(t2, p, t1);                                          \
      for (size_t i = 0; i < N_SHARES; i++) {                                  \
        p[i] = t2[i];                                                          \
      }                                                                        \
    }                                                                          \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      t0[i] = g[i] << (1 << LOG2M1(BW));                                       \
    }                                                                          \
    MSK_uint##BW##_t t2;                                                       \
    MSK_SNI_##BW##b_and(t2, t0, p);                                            \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      g[i] ^= t2[i];                                                           \
      x[i] = a[i] ^ b[i] ^ (g[i] << 1);                                        \
    }                                                                          \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_neg(MSK_uint##BW##_t x, const MSK_uint##BW##_t a) {         \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = a[i];                                                             \
    }                                                                          \
    x[0] = ~x[0];                                                              \
    MSK_##BW##b_semi_add(x, x, 1);                                             \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_semi_sub(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,      \
                            const uint##BW##_t b) {                            \
    MSK_##BW##b_semi_add(x, a, UINT##BW##_MAX - b + 1);                        \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_sub(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,           \
                       const MSK_uint##BW##_t b) {                             \
    MSK_uint##BW##_t t0;                                                       \
    MSK_##BW##b_neg(t0, b);                                                    \
    MSK_##BW##b_add(x, a, t0);                                                 \
  }

INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_eq(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,            \
                      const MSK_uint##BW##_t b) {                              \
    MSK_##BW##b_xor(x, a, b);                                                  \
    for (size_t i = 4 * sizeof(uint##BW##_t); i != 0; i /= 2) {                \
      MSK_uint##BW##_t t0;                                                     \
      MSK_uint##BW##_t t1;                                                     \
      MSK_##BW##b_semi_and(t0, x, (((uint##BW##_t)1 << i) - 1));               \
      MSK_##BW##b_right_shift(t1, x, i);                                       \
                                                                               \
      t0[0] ^= (((uint##BW##_t)1 << i) - 1);                                   \
      t1[0] ^= (((uint##BW##_t)1 << i) - 1);                                   \
                                                                               \
      MSK_SNI_##BW##b_and(x, t0, t1);                                          \
      x[0] ^= (((uint##BW##_t)1 << i) - 1);                                    \
    }                                                                          \
  }

INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_if(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,            \
                      const MSK_uint##BW##_t b, const MSK_uint##BW##_t c) {    \
    MSK_uint##BW##_t t0;                                                       \
    MSK_uint##BW##_t t1;                                                       \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      t0[i] = (c[i] & 1) * (uint##BW##_t)(-1);                                 \
    }                                                                          \
                                                                               \
    MSK_SNI_##BW##b_and(x, b, t0);                                             \
    t0[0] ^= (uint##BW##_t)(-1);                                               \
                                                                               \
    MSK_SNI_##BW##b_and(t1, a, t0);                                            \
                                                                               \
    for (size_t i = 0; i < N_SHARES; i++)                                      \
      x[i] ^= t1[i];                                                           \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW, BWUP)                                                            \
  void MSK_##BW##b_max(MSK_uint##BW##_t x, const MSK_uint##BW##_t a,           \
                       const MSK_uint##BW##_t b) {                             \
    MSK_uint##BWUP##_t a_up;                                                   \
    MSK_uint##BWUP##_t b_up;                                                   \
    MSK_uint##BWUP##_t diff_up;                                                \
    MSK_uint##BWUP##_t t0;                                                     \
    MSK_uint##BW##_t t1;                                                       \
                                                                               \
    MSK_##BW##b_to_##BWUP##b_copy(a_up, a);                                    \
    MSK_##BW##b_to_##BWUP##b_copy(b_up, b);                                    \
    MSK_##BWUP##b_sub(diff_up, a_up, b_up);                                    \
    MSK_##BWUP##b_right_shift(t0, diff_up, ((BWUP) - 1));                      \
    MSK_##BWUP##b_to_##BW##b_copy(t1, t0);                                     \
    MSK_##BW##b_if(x, a, b, t1);                                               \
  }
INSTANTIATE_M816_1632_3264
#undef M

#define M(BWFROM, BWTO)                                                        \
  void MSK_##BWFROM##b_to_##BWTO##b_copy(MSK_uint##BWTO##_t x,                 \
                                         const MSK_uint##BWFROM##_t a) {       \
    for (size_t i = 0; i < N_SHARES; i++) {                                    \
      x[i] = (uint##BWTO##_t)a[i];                                             \
    }                                                                          \
  }
INSTANTIATE_2DX_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_to_##BW##b_copy(MSK_uint##BW##_t x,                         \
                                   const MSK_uint##BW##_t a) {                 \
    memcpy(x, a, N_SHARES * sizeof(uint##BW##_t));                             \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BWFROM, BWTO)                                                        \
  void MSK_##BWFROM##b_to_##BWTO##b_copy_array(MSK_uint##BWTO##_t x[],         \
                                               const MSK_uint##BWFROM##_t a[], \
                                               const size_t size) {            \
    for (size_t i = 0; i < size; i++) {                                        \
      MSK_##BWFROM##b_to_##BWTO##b_copy(x[i], a[i]);                           \
    }                                                                          \
  }
INSTANTIATE_2DX_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_to_##BW##b_copy_array(                                      \
      MSK_uint##BW##_t x[], const MSK_uint##BW##_t a[], const size_t size) {   \
    memcpy(x, a, size *N_SHARES * sizeof(uint##BW##_t));                       \
  }
INSTANTIATE_M8_16_32_64
#undef M

/**********************************************************************
 * Conversions between arithletic and boolean masking (64 bits only)  *
 **********************************************************************/

// ======================================================================================
// // GADGETS FROM DEMANGE AND ROSSI's MASKED BIKE IMPLEMENTATION Available at:
// https://github.com/loicdemange/masked_BIKE_code/blob/main/BIKE.c

/**
 * @brief XOR shares of 64 bits masked value
 *
 * @return x[0] XOR ... XOR x[n-1]
 * @param[in] [x] 64 bits masked value
 * @param[in] n number of shares of x
 */
uint64_t xorall(const uint64_t *const x, const size_t n) {
  uint64_t r = 0;
  for (size_t i = 0; i < n; i++) {
    r ^= x[i];
  }
  return r;
}

/**
 * @brief ADD shares of 64 bits masked value
 *
 * @return x[0] + ... + x[n-1]
 * @param[in] [x] 64 bits masked value
 * @param[in] n number of shares of x
 */
uint64_t addall(const uint64_t *const x, const size_t n) {
  uint64_t r = 0;
  for (size_t i = 0; i < n; i++) {
    r += x[i];
  }
  return r;
}

/**
 * @brief Expand masked value by doubling number of shares
 *
 * "Algorithm 5" from
 * http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf
 *
 * @param[out] t_exp Expanded and refreshed copy of t
 * @param[in]  t value to expand
 * @param[in]  len_t number of shares of input value
 * @param[in]  n number of shares of expanded output
 */
void expand(uint64_t *const t_exp, const uint64_t *const t, const size_t len_t,
            const size_t n) {
  memcpy(t_exp, t, len_t * 8);
  _MSK_SNI_64b_inplace_refresh(t_exp, n);
}

void MSK_arithmetic_to_boolean(uint64_t *const z, const uint64_t *const a,
                               const size_t n) {
  // n : len of a
  if (n == 1) {
    z[0] = a[0];
    return;
  }
  uint8_t dn = n / 2;

  uint64_t x[dn];
  uint64_t y[dn + 1];

  uint64_t xp[n];
  uint64_t yp[n];
  memset(xp, 0, 8 * n);
  memset(yp, 0, 8 * n);

  MSK_arithmetic_to_boolean(x, a, dn);
  expand(xp, x, dn, n);

  MSK_arithmetic_to_boolean(y, a + dn, n - dn);
  expand(yp, y, n - dn, n);

  _MSK_64b_add(z, xp, yp, n);
}

/**
 * @brief Refresh and XOR shares of masked value
 *
 * "Algorithm 7" from
 * http://www.crypto-uni.lu/jscoron/publications/secconvorder.pdf
 *
 * @return x[0] XOR ... XOR x[n-1]
 * @param[inout] [x] masked 64 bits input value (refreshed)
 * @param[in] n number of shares of x
 */
uint64_t full_xor(uint64_t *const x, const size_t n) {

  for (size_t i = 0; i < n; i++) {
    _MSK_SNI_64b_inplace_refresh(x, n);
  }
  return xorall(x, n);
}

void MSK_boolean_to_arithmetic(uint64_t *const z, const uint64_t *const a,
                               const size_t n) {

  uint64_t A[n];
  uint64_t y[n];
  uint64_t w[n];

  A[n - 1] = 0;
  for (size_t i = 0; i < n - 1; i++) {
    z[i] = next_64();
    A[i] = -z[i];
  }

  MSK_arithmetic_to_boolean(y, A, n);

  _MSK_64b_add(w, a, y, n);
  z[n - 1] = full_xor(w, n);
}

/**********************************************************************
 * Debugging helper functions                                         *
 **********************************************************************/

#define M(BW)                                                                  \
  void fprintf_uint##BW##_t_array(                                             \
      FILE *fp, const char *const prefix, const uint##BW##_t *const a,         \
      const size_t size, const char *const postfix) {                          \
    size_t u = size * ((BW) / 8);                                              \
    uint8_t v[u];                                                              \
                                                                               \
    memcpy(v, a, u);                                                           \
    fprintf(fp, "%s", prefix);                                                 \
    for (size_t i = 0; i < u; i++) {                                           \
      fprintf(fp, "%02" PRIx8, v[i]);                                          \
    }                                                                          \
    fprintf(fp, "%s", postfix);                                                \
  }
INSTANTIATE_M8_16_32_64
#undef M

#define M(BW)                                                                  \
  void MSK_##BW##b_fprintf_array(                                              \
      FILE *fp, const char *const prefix, const MSK_uint##BW##_t *const a,     \
      const size_t size, const char *const postfix) {                          \
    uint##BW##_t u[size];                                                      \
                                                                               \
    MSK_##BW##b_unmask_array(u, a, size);                                      \
    fprintf_uint##BW##_t_array(fp, prefix, u, size, postfix);                  \
  }
INSTANTIATE_M8_16_32_64
#undef M

// vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab textwidth=0:
