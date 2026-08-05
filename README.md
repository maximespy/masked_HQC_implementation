# Masked implementation of Hamming Quasi-Cyclic (HQC)

This repository provides a C implementation of the Hamming Quasi-Cyclic (HQC) code-based post-quantum Key Encapsulation Mechanism (KEM) selected by the NIST.
The HQC version is the one submitted to NIST by the HQC authors the 19th of February 2025.
This C implementation is protected against side-channel attacks by masking all sensitive variables and operations.
The masking order, the HQC security level, and several other parameters are configurable at compile time.

## Authors

- **Maxime SPYROPOULOS** — [@maximespy](https://github.com/maximespy)  
- **Guillaume GOY** — [@guillaume-goy](https://github.com/guillaume-goy)
- **Nicolas ARAGON**
- **Philippe GABORIT**
- **Renaud PACALET**
- **Fabrice PERION**
- **David VIGILANT**

## Structure

.
├── Doxyfile             Doxygen file
├── KATs/                Known Answer Tests for HQC-128, HQC-192 and HQC-256
│   ├── hqc-128_kat.rsp
│   ├── hqc-192_kat.rsp
│   └── hqc-256_kat.rsp
├── Makefile             GNU-make Makefile to automate build and tests
├── README.md            This file
└── src/                 C source files

## Compilation

The provided Makefile has a `help` default goal:

```bash
$ make
Usage: make [GOAL] [VAR=VAL ...]

Goals

    help       Print this help message (default goal)
    kat        Build the kat executable defined by the compile-time variables
               (see below)
    kats       Build and run all Known Answer Tests (KATs) specified by the
               KATS_X variables (see below)
    doc        Compile documentation
    clean      Delete all generated files

Variables (valid values): current value

  Compile-time variables for kat build

    S          Security level (128 192 256): 128
    M          Masking order (positive integer): 1
    T          Enable STM32F4xx True Random Number Generator (0:no, 1:yes): 0
    V          Verbosity level at run time (0:none, 1:partial, 2:full). Use
               with care, full verbose output contains all outputs of the
               Pseudo Random Number Generator (PRNG) used for masking, which
               can be huge: 0
                           
  KATs compile-time variables           
                           
    KATS_S     Security levels to build (subset of 128 192 256): 128 192 256
    KATS_M     Masking orders to build (set of positive integers): 1 2 3
    KATS_T     STM32F4xx TRNG modes to build (subset of 0 1): 0

  KATs run-time variables           
                           
    KATS_U     Set to non-empty string to generated unmasked salts for accurate
               performance measurements. This disables ciphertext and shared
               secret checks, because the salts are different from the KATs,
               but encapsulated and decapsulated shared secrets are still
               compared: 
    KATS_D     Directory containing KAT files: ./KATs
    KATS_R     Masking PRNGs to test (subset of 0 1 2 3 4 5): 0
               - 0: default, fast constant 0 (use for speed tests)
               - 1: constant 0 (use for leakage T-tests baseline)
               - 2: XOSHIRO256** (use for functional tests of masked version)
               - 3: SHAKE256 (use for functional tests of masked version)
               - 4: /dev/urandom (use for functional tests of masked version)
               - 5: STM32F4xx True Random Number Generator (use for leakage T-tests)
    KATS_L     Size of masking randomness pool in 64-bits words: 0
    KATS_O     Masking randomness pool behaviour when KATS_L > 0 and the pool
               is empty (subset of 0 1 2): 0
               - 0: default, automatically refill from the selected PRNG
               - 1: loop over the pool (randomness reuse)
               - 2: abort execution
    KATS_N     Number of KATs to test (1 to 100): 100

  Other variables

    BTVL       Verbosity level at build time (0:quiet, 1:partial, 2:full): 1
    BUILDROOT  Root build directory: build
```

Examples:

Build a `kat` executable for security level HQC-128, masking order 1, no STM32F4xx TRNG support, and no verbostity at run-time:

```
$ make kat S=128 M=1 T=0 V=0
[CC] build/s128m1/barrett.o
...
[CC] build/s128m1/xoshiro256starstar.o
[LD] build/s128m1/kat
```

Run it on the first 5 official KATs with the XOSHIRO256\*\* PRNG for masking, no masking randomness pool, and store the collected statistics in `stats.txt`:

```
$ build/s128m1/kat -n 5 -r 2 KATs/hqc-128_kat.rsp stats.txt

All KAT passed!
$ cat stats.txt
# ALGO	 Time (ns)	Random bytes
KEYGEN	  53017240	  43347928
ENCAPS	  70346928	  81685968
DECAPS	 104308082	 118995888
...
```

Build and run the 10 first KATs for the 3 security levels, masking orders 1, 2 and 5, unmasked salts, and using the fast zero masking "PRNG" without masking pool:

```
$ make kats KATS_U=1 KATS_S='128 192 256' KATS_M='1 2 5' KATS_R=0 KATS_L=0 KATS_N=10
[CC] build/s128m1/barrett.o
...
s128m1r0: PASSED. Stats/logs/outputs in build/s128m1r0.{sts/log/out}
...
```
