# ---------------- TOOLS ---------------------

DOXYGEN       := doxygen
DOXYGEN_FLAGS :=
DOXYGEN_FILE  := Doxyfile

# ---------------- PARAMETERS ---------------------

# Valid HQC security levels
VSL           := 128 192 256
# Valid TRNG modes
VT            := 0 1
# Valid run-time verbosity levels
VRTVL         := 0 1 2

# Valid Pseudo Random Number Generator (PRNG) for masking
VPRNG         := 0 1 2 3 4 5
# Valid build-time verbosity levels
VBTVL         := 0 1 2
# Valid masking randomness pool behaviours
VMRPB         := 0 1 2

# COMPILE-TIME PARAMETERS
# HQC security level
S             := 128
# Masking order
M             := 1
# STM32F4xx TRNG disable/enable
T             := 0
# Run-time verbosity level
V             := 0

# Build-time verbosity level
BTVL          := 1

# KATs build-time configurations
# Build kat executable with all 3 security levels
KATS_S        := $(VSL)
# Build kat executable with masking orders 1, 2 and 3
KATS_M        := 1 2 3
# Build kat executable  without STM32F4xx TRNG support
KATS_T        := 0

# KATs run-time configurations
# Generate masked salts
KATS_U        :=
# Directory containing KAT files
KATS_D        := ./KATs
# Run with only fast-constant-zero "PRNG" 
KATS_R        := 0
# Run without PRNG pool
KATS_L        := 0
# Run with automatic PRNG pool refill
KATS_O        := 0
# Run all 100 official KATs
KATS_N        := 100

ifneq ($(BTVL),2)
.SILENT:
endif

#----------------- CHECK PARAMETERS -------------------

ifneq ($(filter-out $(VSL),$S),)
$(error Invalid HQC security level: "$S" (valid values: $(VSL)))
endif

ifneq ($(filter-out $(VT),$T),)
$(error Invalid STM32F4xx TRNG mode: "$T" (valid values: $(VT)))
endif

ifneq ($(filter-out $(VRTVL),$V),)
$(error Invalid run-time verbosity level: "$V" (valid values: $(VRTVL)))
endif

#---------------- DIRECTORIES AND FILES -------------------------------------

DOCDIR    := doc
SRCDIR    := src
BUILDROOT := build
BUILDDIR  := $(BUILDROOT)/s$Sm$M
EXE       := $(BUILDDIR)/kat
SRC       := $(wildcard $(SRCDIR)/*.c)
OBJ       := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))
DEP       := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.d,$(SRC))

#------------------- COMPILATION FLAGS ------------------------------------------------------

CC     := gcc

CFLAGS := -O3 -std=c23 -pedantic -Wall -Wextra -Wpedantic -Wredundant-decls
CFLAGS += -DSECURITY_LEVEL=$S -DMASKING_ORDER=$M -DVERBOSITY_LEVEL=$V -DSTM32F4XX_TRNG=$T
# Comment out the next line to avoid the refreshes needed for MIMOSNI gadgets (instead of NI)
# Warning: without these refreshes there is no formal proof any more that the
# resulting masked HQC implementation is t-probing secure.
CFLAGS += -DMIMOSNI

LDFLAGS :=
LDLIBS  :=

DEPFLAGS  = -MT $@ -MMD -MP -MF $(BUILDDIR)/$*.d
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
ifeq ($(BTVL),1)
LOGCC.c   = printf '[CC] $@\n'
LOGLD.c   = printf '[LD] $@\n'
endif

$(BUILDDIR)/mask.o: CFLAGS += -Wno-maybe-uninitialized

#----------------- PHONY TARGETS -------------------------------------------------------

.PHONY: help kat doc clean
.DEFAULT_GOAL := help

define HELP_MESSAGE
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

    S          Security level ($(VSL)): $S
    M          Masking order (positive integer): $M
    T          Enable STM32F4xx True Random Number Generator (0:no, 1:yes): $T
    V          Verbosity level at run time (0:none, 1:partial, 2:full). Use
               with care, full verbose output contains all outputs of the
               Pseudo Random Number Generator (PRNG) used for masking, which
               can be huge: $V
                           
  KATs compile-time variables           
                           
    KATS_S     Security levels to build (subset of $(VSL)): $(KATS_S)
    KATS_M     Masking orders to build (set of positive integers): $(KATS_M)
    KATS_T     STM32F4xx TRNG modes to build (subset of $(VT)): $(KATS_T)

  KATs run-time variables           
                           
    KATS_U     Set to non-empty string to generated unmasked salts for accurate
               performance measurements. This disables ciphertext and shared
               secret checks, because the salts are different from the KATs,
               but encapsulated and decapsulated shared secrets are still
               compared: $(KATS_U)
    KATS_D     Directory containing KAT files: $(KATS_D)
    KATS_R     Masking PRNGs to test (subset of $(VPRNG)): $(KATS_R)
               - 0: default, fast constant 0 (use for speed tests)
               - 1: constant 0 (use for leakage T-tests baseline)
               - 2: XOSHIRO256** (use for functional tests of masked version)
               - 3: SHAKE256 (use for functional tests of masked version)
               - 4: /dev/urandom (use for functional tests of masked version)
               - 5: STM32F4xx True Random Number Generator (use for leakage T-tests)
    KATS_L     Size of masking randomness pool in 64-bits words: $(KATS_L)
    KATS_O     Masking randomness pool behaviour when KATS_L > 0 and the pool
               is empty (subset of $(VMRPB)): $(KATS_O)
               - 0: default, automatically refill from the selected PRNG
               - 1: loop over the pool (randomness reuse)
               - 2: abort execution
    KATS_N     Number of KATs to test (1 to 100): $(KATS_N)

  Other variables

    BTVL       Verbosity level at build time (0:quiet, 1:partial, 2:full): $(BTVL)
    BUILDROOT  Root build directory: $(BUILDROOT)

endef
export HELP_MESSAGE

help:
	printf '%s' "$$HELP_MESSAGE"

kat: $(EXE)

#---------------- TARGETS -------------------------------

$(OBJ): $(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)/%.d | $(BUILDDIR)
	$(LOGCC.c)
	$(COMPILE.c) && touch $@

$(DEP):

include $(wildcard $(DEP))

$(EXE): $(OBJ)
	$(LOGLD.c)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILDDIR):
	mkdir -p $@

# $1: security level
# $2: masking order
# $3: masking PRNG
define KATS_RULE
.PHONY: $(BUILDROOT)/s$1m$2r$3.sts

kats: $(BUILDROOT)/s$1m$2r$3.sts

$(BUILDROOT)/s$1m$2r$3.sts:
	$$(MAKE) --no-print-directory kat S=$1 M=$2 V=$$V BTVL=$$(BTVL) BUILDROOT=$$(BUILDROOT)
	if $(BUILDROOT)/s$1m$2/kat $$(if $$(KATS_U),-u,) -n $$(KATS_N) -r $3 -l $$(KATS_L) -m $$(KATS_O) $$(KATS_D)/hqc-$1_kat.rsp $$@ 2> '$$(patsubst %sts,%log,$$@)' > '$$(patsubst %sts,%out,$$@)'; then \
		printf '\e[0;32m%s: PASSED. Stats/logs/outputs in %s{sts/log/out}\e[0m\n' 's$1m$2r$3' '$$(patsubst %sts,%,$$@)'; \
	else \
		printf '\e[0;31m%s: FAILED. Stats/logs/outputs in %s{sts/log/out}\e[0m\n' 's$1m$2r$3' '$$(patsubst %sts,%,$$@)'; \
	fi
endef
$(foreach s,$(KATS_S),$(foreach m,$(KATS_M),$(foreach r,$(KATS_R),$(eval $(call KATS_RULE,$s,$m,$r)))))

doc:
	$(DOXYGEN) $(DOXYGEN_FLAGS) $(DOXYGEN_FILE)
	$(MAKE) -C $(DOCDIR)/latex
	@printf '\n\nPDF reference manual in %s\n' "$(DOCDIR)/latex/refman.pdf"

clean:
	rm -rf $(BUILDROOT) $(DOCDIR)
