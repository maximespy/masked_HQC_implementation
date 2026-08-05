/**
 * @file prng.c
 * @brief Configurable wrapper around a Pseudo Random Number Generator (PRNG).
 *
 * Static global variable prng_state contains two stores of random bytes: an
 * array of two uint64_t (pool) and another array of pool2_size uint64_t
 * (pool2). The stores are populated with freshly drawn random bytes from the
 * selected PRNG.
 *
 * pool is the first stage of the PRNG wrapper and can be seen as a string of
 * 16 bytes:
 *
 * MSB  pool[1]  LSB  MSB  pool[0]  LSB
 * +-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+
 * |7|6|5|4|3|2|1|0|  |7|6|5|4|3|2|1|0|
 * +-+-+-+-+-+-+-+-+  +-+-+-+-+-+-+-+-+
 * -------- shifting direction ------->
 *
 * The next_8, next_16, next_32 and next_64 functions return the 1, 2, 4 or 8
 * rightmost bytes of pool, respectively. pool is then shifted to the right by
 * 1, 2, 4 or 8 bytes. After the shift, if pool contains less than 8 bytes, 8
 * new fresh bytes are added to the left such that it always contains between 8
 * and 16 bytes. The 8 rightmost bytes are always fresh random bytes.
 *
 * pool2 is the second stage of the wrapper. It can contain up to 8*pool2_size
 * random bytes, prepared in advance. If it is not empty, when 8 fresh random
 * bytes are added to pool, they are copied from pool2, instead of being drawn
 * from the PRNG. If pool2 is large enough and filled in advance, it can
 * simplify performance or security evaluations by avoiding calls to the PRNG
 * for some time.
 *
 * pool2_idx is the index of the next fresh uint64_t in pool2; it is
 * incremented each time 8 bytes are copied from pool2 to pool, to point to the
 * next fresh uint64_t. pool2 is full when pool2_idx==0, and empty when
 * pool2_idx==pool2_size. pool2 is refilled by calling prng_pool2_fill.
 */

#include "prng.h"
#include "shake_prng.h"
#include "xoshiro256starstar.h"

#if STM32F4XX_TRNG != 0
#include "stm32f4xx_hal.h"

static RNG_HandleTypeDef HardwareRNGHandle;
#endif

/**
 * @brief PRNG_LOG macro print only if VERBOSITY_LEVEL equals 2
 */
#if VERBOSITY_LEVEL == 2
#define PRNG_LOG(fp, format, ...)                                              \
  do {                                                                         \
    fprintf((fp), (format)__VA_OPT__(, ) __VA_ARGS__);                         \
  } while (0)
#else
#define PRNG_LOG(fp, format, ...)                                              \
  do {                                                                         \
  } while (0)
#endif

/**
 * @brief Wrapper state
 */
typedef struct {
  prng_type_t type;          /**< Type of PRNG (see prng.h for valid values) */
  prng_mode_t mode;          /**< Mode of PRNG (see prng.h for valid values) */
  uint64_t pool[2];          /**< First stage pool of 16 random bytes */
  size_t total_byte_counter; /**< Number of random bytes added to pool since
                                initialization */
  size_t byte_counter;       /**< Number of random bytes currently in pool */
  FILE *urandom_fp;          /**< File pointer to /dev/urandom */
  uint64_t (*next)(void);    /**< Draw 8 new fresh bytes from PRNG */
  seedexpander_ctx shake_prng_ctx; /**< Context for SHAKE-256 PRNG */

  size_t pool2_size; /**< Size of pool2 (number of uint64_t) */
  size_t pool2_idx;  /**< Index of next fresh entry in pool2 */
  uint64_t *pool2;   /**< Second stage pool of pool2_size random uint64_t */
} prng_state_t;

/**
 * @brief rapper state
 */
static prng_state_t prng_state;

/**
 * @brief Zero randomness, used as non-masked reference for T-tests
 *
 * @return 64-bits zero
 */
uint64_t masking_zero_next(void);

/**
 * @brief SHAKE PRNG
 *
 * @return Next 64-bits random number from SHAKE-256 PRNG
 */
uint64_t masking_shake_next(void);

/**
 * @brief /dev/urandom PRNG
 *
 * @return Next 64-bits random number from /dev/urandom
 */
uint64_t masking_devurandom_next(void);

#if STM32F4XX_TRNG != 0
/**
 * @brief STM32F4xx True Random Number Generator (TRNG)
 *
 * @return Next 64-bits random number from TRNG
 */
uint64_t masking_stm32f4xxtrng_next(void);
#endif

/**
 * @brief Fill pool from PRNG such that it contains 16 fresh random bytes.
 *
 * If pool already contains N random bytes, N%8 bytes are overwritten but the
 * lost random bytes are not counted in the total used randomness.
 */
void prng_pool_fill(void) {
  if (prng_state.byte_counter < 16) {
    prng_state.pool[1] = prng_state.next(); // Draw 8 bytes
    if (prng_state.byte_counter < 8) {
      prng_state.pool[0] = prng_state.next(); // Draw 8 bytes
    }
    prng_state.total_byte_counter += 16 - prng_state.byte_counter;
    prng_state.byte_counter = 16;
  }
}

/**
 * @brief Fill pool2 from PRNG such that it contains 8*pool2_size fresh random
 * bytes.
 */
void prng_pool2_fill(void) {
  for (size_t i = 0; i < prng_state.pool2_idx; i++) {
    prng_state.pool2[i] = prng_state.next(); // Draw 8 bytes
  }
  prng_state.pool2_idx = 0;
}

uint64_t masking_shake_next(void) {
  uint64_t result;
  seedexpander((uint8_t *)&result, &prng_state.shake_prng_ctx, 8);
  return result;
}

void prng_init(const prng_type_t type, const size_t pool2_size,
               const prng_mode_t mode) {
  FILE *fp;
  uint8_t seed[32];

  prng_state.type = type; // PRNG type
  prng_state.mode = mode; // PRNG mode
  prng_state.total_byte_counter = 0;
  prng_state.byte_counter = 0;
  prng_state.urandom_fp = NULL; // Default if type != PRNG_DEVURANDOM
  switch (type) {
  case PRNG_ZERO_FAST: // intentionally fall through PRNG_ZERO case
  case PRNG_ZERO:
    prng_state.next = &masking_zero_next;
    break;
  case PRNG_XOSHIRO:
    seed_xoshiro();
    prng_state.next = &xoshiro_next;
    break;
  case PRNG_SHAKE:
    fp = fopen("/dev/urandom", "r");
    if (fp == NULL) {
      fprintf(stderr, "Fatal error: cannot open /dev/urandom for reading.\n");
      exit(EXIT_FAILURE);
    }
    if (fread(seed, 1, 32, fp) != 32) {
      fprintf(stderr,
              "Fatal error: cannot read /dev/urandom for SHAKE seeding.\n");
      exit(EXIT_FAILURE);
    }
    fclose(fp);
    seedexpander_init(&prng_state.shake_prng_ctx, seed, 32);
    prng_state.next = &masking_shake_next;
    break;
  case PRNG_DEVURANDOM:
    prng_state.urandom_fp = fopen("/dev/urandom", "r");
    if (prng_state.urandom_fp == NULL) {
      fprintf(stderr, "Fatal error: cannot open /dev/urandom for reading.\n");
      exit(EXIT_FAILURE);
    }
    prng_state.next = &masking_devurandom_next;
    break;
#if STM32F4XX_TRNG != 0
  case PRNG_STM32F4XX_TRNG:
    HardwareRNGHandle.Instance = RNG;
    HAL_StatusTypeDef hStatus = HAL_RNG_Init(&(HardwareRNGHandle));
    if (hStatus != HAL_OK) {
      fprintf(stderr, "Fatal error: cannot initialize STM32F4xx TRNG.\n");
      exit(EXIT_FAILURE);
    }
    prng_state.next = &masking_stm32f4xxtrng_next;
    break;
#endif
  default:
    fprintf(stderr, "%u: invalid PRNG type.\n", type);
    exit(EXIT_FAILURE);
    break;
  }
  prng_state.pool2_size = pool2_size; // Size of pool2
  prng_state.pool2_idx = 0;           // Index in pool2
  if (pool2_size > 0) {
    prng_state.pool2 = calloc(pool2_size, sizeof(uint64_t)); // Allocate pool
    if (prng_state.pool2 == NULL) {
      fprintf(stderr, "Fatal error: cannot allocate pool2.\n");
      exit(EXIT_FAILURE);
    }
  } else {
    prng_state.pool2 = NULL;
  }
  prng_fill();
}

void prng_fill(void) {
  prng_pool_fill();
  prng_pool2_fill();
}

uint64_t prng_amount(void) {
  return prng_state.total_byte_counter - prng_state.byte_counter;
}

void prng_close(void) {
  if (prng_state.type == PRNG_DEVURANDOM) {
    fclose(prng_state.urandom_fp);
  }
  if (prng_state.pool2_size > 0) {
    free(prng_state.pool2);
  }
}

/**
 * @brief Get 8 bytes from pool2.
 * If zero-size pool2, use PRNG, else use pool2 (after refill if empty).
 */
uint64_t prng_pool2_next(void) {
  if (prng_state.pool2_size == 0) { // If zero-size pool2
    return prng_state.next();       // Use PRNG
  }
  if (prng_state.pool2_idx >= prng_state.pool2_size) { // If pool2 empty
    switch (prng_state.mode) {
    case PRNG_AUTO_REFILL:
      prng_pool2_fill(); // Refill pool2
#if VERBOSITY_LEVEL > 0
      fprintf(stderr, "PRNG: pool2 empty, auto filling\n");
#endif
      break;
    case PRNG_LOOP:
      prng_state.pool2_idx = 0; // Loop over pool2
#if VERBOSITY_LEVEL > 0
      fprintf(stderr, "PRNG: pool2 empty, looping over\n");
#endif
      break;
    case PRNG_CRASH:
      fprintf(stderr, "PRNG: pool2 empty, aborting\n");
      exit(1);
      break;
    }
  }
  return prng_state.pool2[prng_state.pool2_idx++]; // Get 8 bytes from pool2
}

/**
 * @brief Shift pool by num_bytes bytes to the right.
 *
 * If there are less than 8 bytes in pool after the shift, copy 8 new bytes
 * from pool2 to pool or draw 8 fresh bytes from PRNG.
 *
 * @param[in] num_bytes Number of bytes to shift
 */
void prng_pool_shift(size_t num_bytes) {
  // Shift pool[0] by num_bytes to the right, the num_bytes rightmost bytes of
  // pool[1] enter on the left.
  prng_state.pool[0] = (prng_state.pool[1] << (64 - 8 * num_bytes)) |
                       (prng_state.pool[0] >> (8 * num_bytes));
  // Shift pool[1] by num_bytes to the right.
  prng_state.pool[1] >>= (8 * num_bytes);
  prng_state.byte_counter -= num_bytes; // num_bytes less random bytes in pool
  if (prng_state.byte_counter < 8) {    // If less than 8 random bytes in pool
    // Store 8 new random bytes in pool[1] Right shift the new random bytes to
    // fill pool[0].
    prng_state.pool[1] = prng_pool2_next();
    prng_state.pool[0] |= (prng_state.pool[1] << (8 * prng_state.byte_counter));
    prng_state.pool[1] >>= (64 - 8 * prng_state.byte_counter);
    prng_state.total_byte_counter += 8; // Update total number of drawn bytes
    prng_state.byte_counter += 8;       // Update number of random bytes in pool
  }
}

#define M(bits)                                                                \
  uint##bits##_t next_##bits(void) {                                           \
    if (prng_state.type == PRNG_ZERO_FAST) {                                   \
      prng_state.total_byte_counter += (bits) / 8;                             \
      return 0;                                                                \
    }                                                                          \
                                                                               \
    uint##bits##_t r = prng_state.pool[0] & (UINT64_MAX >> (64 - (bits)));     \
    prng_pool_shift((bits) / 8);                                               \
                                                                               \
    PRNG_LOG(stderr, "%0*" PRIx##bits "\n", (bits) / 4, r);                    \
    return r;                                                                  \
  }
M(8)
M(16)
M(32)
M(64)

uint64_t masking_zero_next() { return 0; }

uint64_t masking_devurandom_next() {
  uint64_t res;

  if (fread(&res, 8, 1, prng_state.urandom_fp) != 1) {
    fprintf(stderr, "Fatal error: cannot read /dev/urandom.\n");
    exit(EXIT_FAILURE);
  }
  return res;
}

#if STM32F4XX_TRNG != 0
uint64_t masking_stm32f4xxtrng_next(void) {
  uint64_t rnd;

  rnd = (uint64_t)HAL_RNG_GetRandomNumber(&(HardwareRNGHandle));
  rnd = (rnd << 32) | (uint64_t)HAL_RNG_GetRandomNumber(&(HardwareRNGHandle));
  return rnd;
}
#endif
