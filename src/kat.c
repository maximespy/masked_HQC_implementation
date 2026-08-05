#define _GNU_SOURCE

#include <inttypes.h>
#include <regex.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "kem.h"
#include "parameters.h"
#include "prng.h"
#include "shake_prng.h"

#define SEED_SIZE 48

typedef struct {
  unsigned int count;
  uint8_t seed[SEED_SIZE];
  uint8_t pk[PUBLIC_KEY_BYTES];
  uint8_t sk[SECRET_KEY_BYTES];
  uint8_t ct[CIPHERTEXT_BYTES];
  uint8_t ss[SHARED_SECRET_BYTES];
  uint8_t result;
} test_vector_t;

static const char *const regex =
    "^([[:alpha:]]+)[[:space:]]*[=][[:space:]]*([[:xdigit:]]+)\n$";
static regex_t preg;
static regmatch_t pmatch[3];
static char *line = NULL;
static size_t line_length = 0;
static const char *const token[6] = {"count", "seed", "pk", "sk", "ct", "ss"};
static const size_t byte_array_size[5] = {SEED_SIZE, PUBLIC_KEY_BYTES,
                                          SECRET_KEY_BYTES, CIPHERTEXT_BYTES,
                                          SHARED_SECRET_BYTES};
static const char *const usage = "\
Usage: %s [OPTIONS] KAT STATS\n\
\n\
Description:\n\
  Run Known Answer Tests (KATs), record computation times and randomness consumption\n\
\n\
Parameters:\n\
  KAT:  file containing the Known Answer Tests (\"-\" for standard input)\n\
  STAT: output file for runtime computation times and randomness consumption (\"-\" for standard output)\n\
\n\
Options:\n\
  -h       print help and exit\n\
  -n NUM   number of KATs to run (all by default)\n\
  -u       generate salts unmasked\n\
  -r PRNG  Pseudo Random Numbers Generator for masking:\n\
             0: default, fast constant 0 (use for speed tests)\n\
             1: constant 0 (use for leakage T-tests baseline)\n\
             2: XOSHIRO256**\n\
             3: SHAKE256\n\
             4: /dev/urandom\n\
  -l LEN   size (in 64-bits words) of masking pool (default 0)\n\
  -m MODE  masking pool mode when LEN > 0:\n\
             0: default, automatically refill masking pool when empty\n\
             1: loop over masking pool when reaching the end\n\
             2: abort execution when reading past the end of masking pool\n\
";

int get_next_test_vector(test_vector_t *const tv, FILE *const fp);
void fprintf_test_vector(FILE *const fp, const test_vector_t tv);
int UNMSK_check(const uint8_t *const actual, const uint8_t *const reference,
                const size_t size, const char *const name,
                const char *const short_name);
int MSK_check(const MSK_uint8_t actual[], const uint8_t *const reference,
              const size_t size, const char *const name,
              const char *const short_name);

int main(int argc, char **argv) {

  FILE *fp, *sts;
  test_vector_t tv;
  MSK_uint8_t MSK_seed[SEED_SIZE];
  uint8_t seed[SEED_SIZE];
  uint8_t pk[PUBLIC_KEY_BYTES];
  MSK_uint8_t sk[SECRET_KEY_BYTES];
  uint8_t ct[CIPHERTEXT_BYTES];
  MSK_uint8_t key1[SHARED_SECRET_BYTES];
  MSK_uint8_t key2[SHARED_SECRET_BYTES];
  MSK_uint8_t result;
  struct timespec ts1, ts2;
  uint64_t t, r, num_kats;
  prng_type_t prng_type;
  prng_mode_t prng_mode;
  size_t prng_pool2_size;
  int opt;
  int unmasked_salts = 0;

  num_kats = UINT64_MAX;
  prng_type = PRNG_ZERO_FAST;
  prng_mode = PRNG_AUTO_REFILL;
  prng_pool2_size = 0;
  while ((opt = getopt(argc, argv, "hun:r:l:m:")) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr, usage, argv[0]);
      exit(EXIT_FAILURE);
      break;
    case 'u':
      unmasked_salts = 1;
      break;
    case 'n':
      num_kats = strtoul(optarg, NULL, 10);
      break;
    case 'r':
      prng_type = strtoul(optarg, NULL, 10);
      break;
    case 'l':
      prng_pool2_size = strtoul(optarg, NULL, 10);
      break;
    case 'm':
      prng_mode = strtoul(optarg, NULL, 10);
      break;
    default: /* '?' */
      fprintf(stderr, usage, argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind != argc - 2) {
    fprintf(stderr, usage, argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strcmp(argv[optind], "-")) {
    fp = fopen(argv[optind], "r");
    if (fp == NULL) {
      fprintf(stderr, "%s: file not found\n", argv[optind]);
      perror("fopen");
      exit(EXIT_FAILURE);
    }
  } else {
    fp = stdin;
  }

  optind += 1;
  if (strcmp(argv[optind], "-")) {
    sts = fopen(argv[optind], "w");
    if (sts == NULL) {
      fprintf(stderr, "%s: could not open file\n", argv[optind]);
      perror("fopen");
      exit(EXIT_FAILURE);
    }
  } else {
    sts = stdout;
  }

  if (regcomp(&preg, regex, REG_EXTENDED | REG_NEWLINE)) {
    fprintf(stderr, "Invalid regular expression: %s\n", regex);
    exit(EXIT_FAILURE);
  }

  tv.result = 0;

  fprintf(sts, "%6s\t%10s\t%10s\n", "# ALGO", "Time (ns)", "Random bytes");

  while ((get_next_test_vector(&tv, fp) != -1) && (num_kats > 0)) {
    int status = 0;

    MSK_8b_mask_array(MSK_seed, tv.seed, SEED_SIZE);
    MSK_shake_prng_init(MSK_seed, NULL, SEED_SIZE, 0);
    shake_prng_init(seed, NULL, SEED_SIZE, 0);
    prng_init(prng_type, prng_pool2_size, prng_mode);

    r = prng_amount();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
    MSK_crypto_kem_keypair(pk, sk);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
    r = prng_amount() - r;
    status |= UNMSK_check(pk, tv.pk, PUBLIC_KEY_BYTES, "public key", "pk");
    status |= MSK_check(sk, tv.sk, SECRET_KEY_BYTES, "secret key", "sk");
    t = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
    fprintf(sts, "%6s\t%10" PRIu64 "\t%10" PRIu64 "\n", "KEYGEN", t, r);

    prng_fill();
    r = prng_amount();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
    MSK_crypto_kem_enc(ct, key1, pk, unmasked_salts);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
    r = prng_amount() - r;
    // Check only if salts are generated with the same masked PRNG
    // as messages.
    if (!unmasked_salts) {
      status |= UNMSK_check(ct, tv.ct, CIPHERTEXT_BYTES, "ciphertext", "ct");
      status |= MSK_check(key1, tv.ss, SHARED_SECRET_BYTES,
                          "encapsulated shared secret", "ss");
    }
    t = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
    fprintf(sts, "%6s\t%10" PRIu64 "\t%10" PRIu64 "\n", "ENCAPS", t, r);

    prng_fill();
    r = prng_amount();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts1);
    MSK_crypto_kem_dec(key2, result, ct, sk);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts2);
    r = prng_amount() - r;
    if (!unmasked_salts) {
      // Check only if salts are generated with the same masked PRNG
      // as messages.
      status |= MSK_check(key2, tv.ss, SHARED_SECRET_BYTES,
                          "decapsulated shared secret", "ss");
    } else {
      // Check that decapsulated shared secret is the same as the encapsulated
      // one.
      uint8_t tmp[SHARED_SECRET_BYTES];
      MSK_8b_unmask_array(tmp, key1, SHARED_SECRET_BYTES);
      status |= MSK_check(key2, tmp, SHARED_SECRET_BYTES,
                          "decapsulated shared secret", "ss");
    }
    status |= MSK_check(&result, &tv.result, 1, "decapsulation success", "res");
    t = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
    fprintf(sts, "%6s\t%10" PRIu64 "\t%10" PRIu64 "\n", "DECAPS", t, r);

    if (status) {
      fprintf(stderr, "\nTest vector:\n\n");
      fprintf_test_vector(stderr, tv);
      exit(EXIT_FAILURE);
    }
    num_kats -= 1;
  }

  fprintf(stdout, "\nAll KAT passed!\n");

  prng_close();
  regfree(&preg);
  free(line);
  fclose(fp);
  fclose(sts);
  exit(EXIT_SUCCESS);
  return 0;
}

void fprintf_test_vector(FILE *const fp, const test_vector_t tv) {
  fprintf(fp, "count = %d\n", tv.count);
  fprintf_uint8_t_array(fp, "\nseed = ", tv.seed, SEED_SIZE, "\n");
  fprintf_uint8_t_array(fp, "\npk = ", tv.pk, PUBLIC_KEY_BYTES, "\n");
  fprintf_uint8_t_array(fp, "\nsk = ", tv.sk, SECRET_KEY_BYTES, "\n");
  fprintf_uint8_t_array(fp, "\nct = ", tv.ct, CIPHERTEXT_BYTES, "\n");
  fprintf_uint8_t_array(fp, "\nss = ", tv.ss, SHARED_SECRET_BYTES, "\n");
}

int UNMSK_check(const uint8_t *const actual, const uint8_t *const reference,
                const size_t size, const char *const name,
                const char *const short_name) {
  if (memcmp(actual, reference, size)) {
    fprintf(stderr, "\n\nReference and computed %s (%s) differ:\n", name,
            short_name);
    fprintf_uint8_t_array(stderr, "\nReference: ", reference, size, "\n");
    fprintf_uint8_t_array(stderr, "\nComputed:  ", actual, size, "\n");
    return 1;
  }
  return 0;
}

int MSK_check(const MSK_uint8_t actual[], const uint8_t *const reference,
              const size_t size, const char *const name,
              const char *const short_name) {
  uint8_t um[size];

  MSK_8b_unmask_array(um, actual, size);
  if (memcmp(um, reference, size)) {
    fprintf(stderr, "\n\nReference and computed %s (%s) differ:\n", name,
            short_name);
    fprintf_uint8_t_array(stderr, "\nReference: ", reference, size, "\n");
    fprintf_uint8_t_array(stderr, "\nComputed:  ", um, size, "\n");
    return 1;
  }
  return 0;
}

int hex_string_to_byte_array(uint8_t *a, const char *const s,
                             const size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (sscanf(s + 2 * i, "%02" SCNx8, a + i) != 1) {
      return -1;
    }
  }
  return 0;
}

int get_next_test_vector(test_vector_t *const tv, FILE *const fp) {
  ssize_t nread;
  uint8_t *p[5] = {tv->seed, tv->pk, tv->sk, tv->ct, tv->ss};

  for (int i = 0; i < 6;) {
    nread = getline(&line, &line_length, fp);
    if (nread == -1) {
      return -1;
    }
    if (regexec(&preg, line, 3, pmatch, 0)) {
      continue;
    }
    if (strncmp(line + pmatch[1].rm_so, token[i],
                pmatch[1].rm_eo - pmatch[1].rm_so)) {
      fprintf(stderr, "Syntax error in KAT file: %s\n", line);
      exit(EXIT_FAILURE);
    }
    if (i == 0) {
      sscanf(line + pmatch[2].rm_so, "%u", &tv->count);
    } else {
      if (hex_string_to_byte_array(p[i - 1], line + pmatch[2].rm_so,
                                   byte_array_size[i - 1])) {
        fprintf(stderr, "Syntax error in KAT file: %s\n", line);
        exit(EXIT_FAILURE);
      }
    }
    i += 1;
  }
  return 0;
}
