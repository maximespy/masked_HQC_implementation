#include <stdlib.h>
#include <string.h>

#include "fips202.h"

/*============================================================*/

// MASKED FIPS from
// https://github.com/fragerar/tches24_masked_Dilithium/blob/master/Dilithium/masked_fips202.c

/**
 * @brief Number of rounds of KeccakF
 */
#define NROUNDS 24

/**
 * @brief Left-rotation of a by offset positions
 */
#define ROL(a, offset) (((a) << (offset)) ^ ((a) >> (64 - (offset))))

/**
 * @brief Masked KeccakF Chi function: r = ((NOT n) AND m) XOR x
 *
 * @param[out] [r] Result (((NOT n) AND m) XOR x), N_SHARES-shares 64-bits
 * boolean-masked
 * @param[in] [n] N_SHARES-shares 64-bits boolean-masked value
 * @param[in] [m] N_SHARES-shares 64-bits boolean-masked value
 * @param[in] [x] N_SHARES-shares 64-bits boolean-masked value
 */
void MSK_KeccakF_Chi(MSK_uint64_t r, const MSK_uint64_t n, const MSK_uint64_t m,
                     const MSK_uint64_t x) {
  MSK_uint64_t tmp;

  MSK_64b_not(tmp, n);
  MSK_SNI_64b_and(r, tmp, m);
  MSK_64b_xor(r, r, x);
}

/**
 * @brief Keccak round constants
 */
static const uint64_t KeccakF_RoundConstants[NROUNDS] = {
    UINT64_C(0x0000000000000001), UINT64_C(0x0000000000008082),
    UINT64_C(0x800000000000808a), UINT64_C(0x8000000080008000),
    UINT64_C(0x000000000000808b), UINT64_C(0x0000000080000001),
    UINT64_C(0x8000000080008081), UINT64_C(0x8000000000008009),
    UINT64_C(0x000000000000008a), UINT64_C(0x0000000000000088),
    UINT64_C(0x0000000080008009), UINT64_C(0x000000008000000a),
    UINT64_C(0x000000008000808b), UINT64_C(0x800000000000008b),
    UINT64_C(0x8000000000008089), UINT64_C(0x8000000000008003),
    UINT64_C(0x8000000000008002), UINT64_C(0x8000000000000080),
    UINT64_C(0x000000000000800a), UINT64_C(0x800000008000000a),
    UINT64_C(0x8000000080008081), UINT64_C(0x8000000000008080),
    UINT64_C(0x0000000080000001), UINT64_C(0x8000000080008008)};

/**
 * @brief Masked Keccak permutation
 *
 * @param[inout] [state] Keccak state, array of 25 N_SHARES-shares 64-bits
 * boolean-masked values
 */
void MSK_KeccakF1600_StatePermute(MSK_uint64_t state[25]) {
  size_t round;

  MSK_uint64_t Aba, Abe, Abi, Abo, Abu;
  MSK_uint64_t Aga, Age, Agi, Ago, Agu;
  MSK_uint64_t Aka, Ake, Aki, Ako, Aku;
  MSK_uint64_t Ama, Ame, Ami, Amo, Amu;
  MSK_uint64_t Asa, Ase, Asi, Aso, Asu;
  MSK_uint64_t BCa, BCe, BCi, BCo, BCu;
  MSK_uint64_t Da, De, Di, Do, Du;
  MSK_uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
  MSK_uint64_t Ega, Ege, Egi, Ego, Egu;
  MSK_uint64_t Eka, Eke, Eki, Eko, Eku;
  MSK_uint64_t Ema, Eme, Emi, Emo, Emu;
  MSK_uint64_t Esa, Ese, Esi, Eso, Esu;

  // copyFromState(A, state)
  MSK_64b_to_64b_copy(Aba, state[0]);
  MSK_64b_to_64b_copy(Abe, state[1]);
  MSK_64b_to_64b_copy(Abi, state[2]);
  MSK_64b_to_64b_copy(Abo, state[3]);
  MSK_64b_to_64b_copy(Abu, state[4]);
  MSK_64b_to_64b_copy(Aga, state[5]);
  MSK_64b_to_64b_copy(Age, state[6]);
  MSK_64b_to_64b_copy(Agi, state[7]);
  MSK_64b_to_64b_copy(Ago, state[8]);
  MSK_64b_to_64b_copy(Agu, state[9]);
  MSK_64b_to_64b_copy(Aka, state[10]);
  MSK_64b_to_64b_copy(Ake, state[11]);
  MSK_64b_to_64b_copy(Aki, state[12]);
  MSK_64b_to_64b_copy(Ako, state[13]);
  MSK_64b_to_64b_copy(Aku, state[14]);
  MSK_64b_to_64b_copy(Ama, state[15]);
  MSK_64b_to_64b_copy(Ame, state[16]);
  MSK_64b_to_64b_copy(Ami, state[17]);
  MSK_64b_to_64b_copy(Amo, state[18]);
  MSK_64b_to_64b_copy(Amu, state[19]);
  MSK_64b_to_64b_copy(Asa, state[20]);
  MSK_64b_to_64b_copy(Ase, state[21]);
  MSK_64b_to_64b_copy(Asi, state[22]);
  MSK_64b_to_64b_copy(Aso, state[23]);
  MSK_64b_to_64b_copy(Asu, state[24]);
  for (round = 0; round < NROUNDS; round += 2) {
    // prepareTheta
    MSK_64b_xor5(BCa, Aba, Aga, Aka, Ama, Asa);
    MSK_64b_xor5(BCe, Abe, Age, Ake, Ame, Ase);
    MSK_64b_xor5(BCi, Abi, Agi, Aki, Ami, Asi);
    MSK_64b_xor5(BCo, Abo, Ago, Ako, Amo, Aso);
    MSK_64b_xor5(BCu, Abu, Agu, Aku, Amu, Asu);

    // thetaRhoPiChiIotaPrepareTheta(round, A, E)
    MSK_64b_left_rotate(Da, BCe, 1);
    MSK_64b_xor(Da, Da, BCu);
    MSK_64b_left_rotate(De, BCi, 1);
    MSK_64b_xor(De, De, BCa);
    MSK_64b_left_rotate(Di, BCo, 1);
    MSK_64b_xor(Di, Di, BCe);
    MSK_64b_left_rotate(Do, BCu, 1);
    MSK_64b_xor(Do, Do, BCi);
    MSK_64b_left_rotate(Du, BCa, 1);
    MSK_64b_xor(Du, Du, BCo);

    MSK_64b_xor(Aba, Aba, Da);
    MSK_64b_to_64b_copy(BCa, Aba);
    MSK_64b_xor(Age, Age, De);
    MSK_64b_left_rotate(BCe, Age, 44);
    MSK_64b_xor(Aki, Aki, Di);
    MSK_64b_left_rotate(BCi, Aki, 43);
    MSK_64b_xor(Amo, Amo, Do);
    MSK_64b_left_rotate(BCo, Amo, 21);
    MSK_64b_xor(Asu, Asu, Du);
    MSK_64b_left_rotate(BCu, Asu, 14);
    MSK_KeccakF_Chi(Eba, BCe, BCi, BCa);
    Eba[0] ^= (uint64_t)KeccakF_RoundConstants[round];
    MSK_KeccakF_Chi(Ebe, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Ebi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Ebo, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Ebu, BCa, BCe, BCu);
    MSK_64b_xor(Abo, Abo, Do);
    MSK_64b_left_rotate(BCa, Abo, 28);
    MSK_64b_xor(Agu, Agu, Du);
    MSK_64b_left_rotate(BCe, Agu, 20);
    MSK_64b_xor(Aka, Aka, Da);
    MSK_64b_left_rotate(BCi, Aka, 3);
    MSK_64b_xor(Ame, Ame, De);
    MSK_64b_left_rotate(BCo, Ame, 45);
    MSK_64b_xor(Asi, Asi, Di);
    MSK_64b_left_rotate(BCu, Asi, 61);
    MSK_KeccakF_Chi(Ega, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Ege, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Egi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Ego, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Egu, BCa, BCe, BCu);
    MSK_64b_xor(Abe, Abe, De);
    MSK_64b_left_rotate(BCa, Abe, 1);
    MSK_64b_xor(Agi, Agi, Di);
    MSK_64b_left_rotate(BCe, Agi, 6);
    MSK_64b_xor(Ako, Ako, Do);
    MSK_64b_left_rotate(BCi, Ako, 25);
    MSK_64b_xor(Amu, Amu, Du);
    MSK_64b_left_rotate(BCo, Amu, 8);
    MSK_64b_xor(Asa, Asa, Da);
    MSK_64b_left_rotate(BCu, Asa, 18);
    MSK_KeccakF_Chi(Eka, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Eke, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Eki, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Eko, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Eku, BCa, BCe, BCu);
    MSK_64b_xor(Abu, Abu, Du);
    MSK_64b_left_rotate(BCa, Abu, 27);
    MSK_64b_xor(Aga, Aga, Da);
    MSK_64b_left_rotate(BCe, Aga, 36);
    MSK_64b_xor(Ake, Ake, De);
    MSK_64b_left_rotate(BCi, Ake, 10);
    MSK_64b_xor(Ami, Ami, Di);
    MSK_64b_left_rotate(BCo, Ami, 15);
    MSK_64b_xor(Aso, Aso, Do);
    MSK_64b_left_rotate(BCu, Aso, 56);
    MSK_KeccakF_Chi(Ema, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Eme, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Emi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Emo, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Emu, BCa, BCe, BCu);
    MSK_64b_xor(Abi, Abi, Di);
    MSK_64b_left_rotate(BCa, Abi, 62);
    MSK_64b_xor(Ago, Ago, Do);
    MSK_64b_left_rotate(BCe, Ago, 55);
    MSK_64b_xor(Aku, Aku, Du);
    MSK_64b_left_rotate(BCi, Aku, 39);
    MSK_64b_xor(Ama, Ama, Da);
    MSK_64b_left_rotate(BCo, Ama, 41);
    MSK_64b_xor(Ase, Ase, De);
    MSK_64b_left_rotate(BCu, Ase, 2);
    MSK_KeccakF_Chi(Esa, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Ese, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Esi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Eso, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Esu, BCa, BCe, BCu);

    // prepareTheta
    MSK_64b_xor5(BCa, Eba, Ega, Eka, Ema, Esa);
    MSK_64b_xor5(BCe, Ebe, Ege, Eke, Eme, Ese);
    MSK_64b_xor5(BCi, Ebi, Egi, Eki, Emi, Esi);
    MSK_64b_xor5(BCo, Ebo, Ego, Eko, Emo, Eso);
    MSK_64b_xor5(BCu, Ebu, Egu, Eku, Emu, Esu);

    // thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
    MSK_64b_left_rotate(Da, BCe, 1);
    MSK_64b_xor(Da, Da, BCu);
    MSK_64b_left_rotate(De, BCi, 1);
    MSK_64b_xor(De, De, BCa);
    MSK_64b_left_rotate(Di, BCo, 1);
    MSK_64b_xor(Di, Di, BCe);
    MSK_64b_left_rotate(Do, BCu, 1);
    MSK_64b_xor(Do, Do, BCi);
    MSK_64b_left_rotate(Du, BCa, 1);
    MSK_64b_xor(Du, Du, BCo);

    MSK_64b_xor(Eba, Eba, Da);
    MSK_64b_to_64b_copy(BCa, Eba);
    MSK_64b_xor(Ege, Ege, De);
    MSK_64b_left_rotate(BCe, Ege, 44);
    MSK_64b_xor(Eki, Eki, Di);
    MSK_64b_left_rotate(BCi, Eki, 43);
    MSK_64b_xor(Emo, Emo, Do);
    MSK_64b_left_rotate(BCo, Emo, 21);
    MSK_64b_xor(Esu, Esu, Du);
    MSK_64b_left_rotate(BCu, Esu, 14);
    MSK_KeccakF_Chi(Aba, BCe, BCi, BCa);
    Aba[0] ^= (uint64_t)KeccakF_RoundConstants[round + 1];
    MSK_KeccakF_Chi(Abe, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Abi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Abo, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Abu, BCa, BCe, BCu);
    MSK_64b_xor(Ebo, Ebo, Do);
    MSK_64b_left_rotate(BCa, Ebo, 28);
    MSK_64b_xor(Egu, Egu, Du);
    MSK_64b_left_rotate(BCe, Egu, 20);
    MSK_64b_xor(Eka, Eka, Da);
    MSK_64b_left_rotate(BCi, Eka, 3);
    MSK_64b_xor(Eme, Eme, De);
    MSK_64b_left_rotate(BCo, Eme, 45);
    MSK_64b_xor(Esi, Esi, Di);
    MSK_64b_left_rotate(BCu, Esi, 61);
    MSK_KeccakF_Chi(Aga, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Age, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Agi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Ago, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Agu, BCa, BCe, BCu);
    MSK_64b_xor(Ebe, Ebe, De);
    MSK_64b_left_rotate(BCa, Ebe, 1);
    MSK_64b_xor(Egi, Egi, Di);
    MSK_64b_left_rotate(BCe, Egi, 6);
    MSK_64b_xor(Eko, Eko, Do);
    MSK_64b_left_rotate(BCi, Eko, 25);
    MSK_64b_xor(Emu, Emu, Du);
    MSK_64b_left_rotate(BCo, Emu, 8);
    MSK_64b_xor(Esa, Esa, Da);
    MSK_64b_left_rotate(BCu, Esa, 18);
    MSK_KeccakF_Chi(Aka, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Ake, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Aki, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Ako, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Aku, BCa, BCe, BCu);
    MSK_64b_xor(Ebu, Ebu, Du);
    MSK_64b_left_rotate(BCa, Ebu, 27);
    MSK_64b_xor(Ega, Ega, Da);
    MSK_64b_left_rotate(BCe, Ega, 36);
    MSK_64b_xor(Eke, Eke, De);
    MSK_64b_left_rotate(BCi, Eke, 10);
    MSK_64b_xor(Emi, Emi, Di);
    MSK_64b_left_rotate(BCo, Emi, 15);
    MSK_64b_xor(Eso, Eso, Do);
    MSK_64b_left_rotate(BCu, Eso, 56);
    MSK_KeccakF_Chi(Ama, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Ame, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Ami, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Amo, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Amu, BCa, BCe, BCu);
    MSK_64b_xor(Ebi, Ebi, Di);
    MSK_64b_left_rotate(BCa, Ebi, 62);
    MSK_64b_xor(Ego, Ego, Do);
    MSK_64b_left_rotate(BCe, Ego, 55);
    MSK_64b_xor(Eku, Eku, Du);
    MSK_64b_left_rotate(BCi, Eku, 39);
    MSK_64b_xor(Ema, Ema, Da);
    MSK_64b_left_rotate(BCo, Ema, 41);
    MSK_64b_xor(Ese, Ese, De);
    MSK_64b_left_rotate(BCu, Ese, 2);
    MSK_KeccakF_Chi(Asa, BCe, BCi, BCa);
    MSK_KeccakF_Chi(Ase, BCi, BCo, BCe);
    MSK_KeccakF_Chi(Asi, BCo, BCu, BCi);
    MSK_KeccakF_Chi(Aso, BCu, BCa, BCo);
    MSK_KeccakF_Chi(Asu, BCa, BCe, BCu);
  }

  // copyToState(state, A)
  MSK_64b_to_64b_copy(state[0], Aba);
  MSK_64b_to_64b_copy(state[1], Abe);
  MSK_64b_to_64b_copy(state[2], Abi);
  MSK_64b_to_64b_copy(state[3], Abo);
  MSK_64b_to_64b_copy(state[4], Abu);
  MSK_64b_to_64b_copy(state[5], Aga);
  MSK_64b_to_64b_copy(state[6], Age);
  MSK_64b_to_64b_copy(state[7], Agi);
  MSK_64b_to_64b_copy(state[8], Ago);
  MSK_64b_to_64b_copy(state[9], Agu);
  MSK_64b_to_64b_copy(state[10], Aka);
  MSK_64b_to_64b_copy(state[11], Ake);
  MSK_64b_to_64b_copy(state[12], Aki);
  MSK_64b_to_64b_copy(state[13], Ako);
  MSK_64b_to_64b_copy(state[14], Aku);
  MSK_64b_to_64b_copy(state[15], Ama);
  MSK_64b_to_64b_copy(state[16], Ame);
  MSK_64b_to_64b_copy(state[17], Ami);
  MSK_64b_to_64b_copy(state[18], Amo);
  MSK_64b_to_64b_copy(state[19], Amu);
  MSK_64b_to_64b_copy(state[20], Asa);
  MSK_64b_to_64b_copy(state[21], Ase);
  MSK_64b_to_64b_copy(state[22], Asi);
  MSK_64b_to_64b_copy(state[23], Aso);
  MSK_64b_to_64b_copy(state[24], Asu);
  return;
}

/*============================================================*/

/**
 * @brief Unmasked Keccak permutation
 *
 * @param[inout] state Keccak state, array of 25 64-bits values
 */
void KeccakF1600_StatePermute(uint64_t *state) {
  int round;

  uint64_t Aba, Abe, Abi, Abo, Abu;
  uint64_t Aga, Age, Agi, Ago, Agu;
  uint64_t Aka, Ake, Aki, Ako, Aku;
  uint64_t Ama, Ame, Ami, Amo, Amu;
  uint64_t Asa, Ase, Asi, Aso, Asu;
  uint64_t BCa, BCe, BCi, BCo, BCu;
  uint64_t Da, De, Di, Do, Du;
  uint64_t Eba, Ebe, Ebi, Ebo, Ebu;
  uint64_t Ega, Ege, Egi, Ego, Egu;
  uint64_t Eka, Eke, Eki, Eko, Eku;
  uint64_t Ema, Eme, Emi, Emo, Emu;
  uint64_t Esa, Ese, Esi, Eso, Esu;

  // copyFromState(A, state)
  Aba = state[0];
  Abe = state[1];
  Abi = state[2];
  Abo = state[3];
  Abu = state[4];
  Aga = state[5];
  Age = state[6];
  Agi = state[7];
  Ago = state[8];
  Agu = state[9];
  Aka = state[10];
  Ake = state[11];
  Aki = state[12];
  Ako = state[13];
  Aku = state[14];
  Ama = state[15];
  Ame = state[16];
  Ami = state[17];
  Amo = state[18];
  Amu = state[19];
  Asa = state[20];
  Ase = state[21];
  Asi = state[22];
  Aso = state[23];
  Asu = state[24];

  for (round = 0; round < NROUNDS; round += 2) {
    // prepareTheta
    BCa = Aba ^ Aga ^ Aka ^ Ama ^ Asa;
    BCe = Abe ^ Age ^ Ake ^ Ame ^ Ase;
    BCi = Abi ^ Agi ^ Aki ^ Ami ^ Asi;
    BCo = Abo ^ Ago ^ Ako ^ Amo ^ Aso;
    BCu = Abu ^ Agu ^ Aku ^ Amu ^ Asu;

    // thetaRhoPiChiIotaPrepareTheta(round, A, E)
    Da = BCu ^ ROL(BCe, 1);
    De = BCa ^ ROL(BCi, 1);
    Di = BCe ^ ROL(BCo, 1);
    Do = BCi ^ ROL(BCu, 1);
    Du = BCo ^ ROL(BCa, 1);

    Aba ^= Da;
    BCa = Aba;
    Age ^= De;
    BCe = ROL(Age, 44);
    Aki ^= Di;
    BCi = ROL(Aki, 43);
    Amo ^= Do;
    BCo = ROL(Amo, 21);
    Asu ^= Du;
    BCu = ROL(Asu, 14);
    Eba = BCa ^ ((~BCe) & BCi);
    Eba ^= KeccakF_RoundConstants[round];
    Ebe = BCe ^ ((~BCi) & BCo);
    Ebi = BCi ^ ((~BCo) & BCu);
    Ebo = BCo ^ ((~BCu) & BCa);
    Ebu = BCu ^ ((~BCa) & BCe);

    Abo ^= Do;
    BCa = ROL(Abo, 28);
    Agu ^= Du;
    BCe = ROL(Agu, 20);
    Aka ^= Da;
    BCi = ROL(Aka, 3);
    Ame ^= De;
    BCo = ROL(Ame, 45);
    Asi ^= Di;
    BCu = ROL(Asi, 61);
    Ega = BCa ^ ((~BCe) & BCi);
    Ege = BCe ^ ((~BCi) & BCo);
    Egi = BCi ^ ((~BCo) & BCu);
    Ego = BCo ^ ((~BCu) & BCa);
    Egu = BCu ^ ((~BCa) & BCe);

    Abe ^= De;
    BCa = ROL(Abe, 1);
    Agi ^= Di;
    BCe = ROL(Agi, 6);
    Ako ^= Do;
    BCi = ROL(Ako, 25);
    Amu ^= Du;
    BCo = ROL(Amu, 8);
    Asa ^= Da;
    BCu = ROL(Asa, 18);
    Eka = BCa ^ ((~BCe) & BCi);
    Eke = BCe ^ ((~BCi) & BCo);
    Eki = BCi ^ ((~BCo) & BCu);
    Eko = BCo ^ ((~BCu) & BCa);
    Eku = BCu ^ ((~BCa) & BCe);

    Abu ^= Du;
    BCa = ROL(Abu, 27);
    Aga ^= Da;
    BCe = ROL(Aga, 36);
    Ake ^= De;
    BCi = ROL(Ake, 10);
    Ami ^= Di;
    BCo = ROL(Ami, 15);
    Aso ^= Do;
    BCu = ROL(Aso, 56);
    Ema = BCa ^ ((~BCe) & BCi);
    Eme = BCe ^ ((~BCi) & BCo);
    Emi = BCi ^ ((~BCo) & BCu);
    Emo = BCo ^ ((~BCu) & BCa);
    Emu = BCu ^ ((~BCa) & BCe);

    Abi ^= Di;
    BCa = ROL(Abi, 62);
    Ago ^= Do;
    BCe = ROL(Ago, 55);
    Aku ^= Du;
    BCi = ROL(Aku, 39);
    Ama ^= Da;
    BCo = ROL(Ama, 41);
    Ase ^= De;
    BCu = ROL(Ase, 2);
    Esa = BCa ^ ((~BCe) & BCi);
    Ese = BCe ^ ((~BCi) & BCo);
    Esi = BCi ^ ((~BCo) & BCu);
    Eso = BCo ^ ((~BCu) & BCa);
    Esu = BCu ^ ((~BCa) & BCe);

    // prepareTheta
    BCa = Eba ^ Ega ^ Eka ^ Ema ^ Esa;
    BCe = Ebe ^ Ege ^ Eke ^ Eme ^ Ese;
    BCi = Ebi ^ Egi ^ Eki ^ Emi ^ Esi;
    BCo = Ebo ^ Ego ^ Eko ^ Emo ^ Eso;
    BCu = Ebu ^ Egu ^ Eku ^ Emu ^ Esu;

    // thetaRhoPiChiIotaPrepareTheta(round+1, E, A)
    Da = BCu ^ ROL(BCe, 1);
    De = BCa ^ ROL(BCi, 1);
    Di = BCe ^ ROL(BCo, 1);
    Do = BCi ^ ROL(BCu, 1);
    Du = BCo ^ ROL(BCa, 1);

    Eba ^= Da;
    BCa = Eba;
    Ege ^= De;
    BCe = ROL(Ege, 44);
    Eki ^= Di;
    BCi = ROL(Eki, 43);
    Emo ^= Do;
    BCo = ROL(Emo, 21);
    Esu ^= Du;
    BCu = ROL(Esu, 14);
    Aba = BCa ^ ((~BCe) & BCi);
    Aba ^= KeccakF_RoundConstants[round + 1];
    Abe = BCe ^ ((~BCi) & BCo);
    Abi = BCi ^ ((~BCo) & BCu);
    Abo = BCo ^ ((~BCu) & BCa);
    Abu = BCu ^ ((~BCa) & BCe);

    Ebo ^= Do;
    BCa = ROL(Ebo, 28);
    Egu ^= Du;
    BCe = ROL(Egu, 20);
    Eka ^= Da;
    BCi = ROL(Eka, 3);
    Eme ^= De;
    BCo = ROL(Eme, 45);
    Esi ^= Di;
    BCu = ROL(Esi, 61);
    Aga = BCa ^ ((~BCe) & BCi);
    Age = BCe ^ ((~BCi) & BCo);
    Agi = BCi ^ ((~BCo) & BCu);
    Ago = BCo ^ ((~BCu) & BCa);
    Agu = BCu ^ ((~BCa) & BCe);

    Ebe ^= De;
    BCa = ROL(Ebe, 1);
    Egi ^= Di;
    BCe = ROL(Egi, 6);
    Eko ^= Do;
    BCi = ROL(Eko, 25);
    Emu ^= Du;
    BCo = ROL(Emu, 8);
    Esa ^= Da;
    BCu = ROL(Esa, 18);
    Aka = BCa ^ ((~BCe) & BCi);
    Ake = BCe ^ ((~BCi) & BCo);
    Aki = BCi ^ ((~BCo) & BCu);
    Ako = BCo ^ ((~BCu) & BCa);
    Aku = BCu ^ ((~BCa) & BCe);

    Ebu ^= Du;
    BCa = ROL(Ebu, 27);
    Ega ^= Da;
    BCe = ROL(Ega, 36);
    Eke ^= De;
    BCi = ROL(Eke, 10);
    Emi ^= Di;
    BCo = ROL(Emi, 15);
    Eso ^= Do;
    BCu = ROL(Eso, 56);
    Ama = BCa ^ ((~BCe) & BCi);
    Ame = BCe ^ ((~BCi) & BCo);
    Ami = BCi ^ ((~BCo) & BCu);
    Amo = BCo ^ ((~BCu) & BCa);
    Amu = BCu ^ ((~BCa) & BCe);

    Ebi ^= Di;
    BCa = ROL(Ebi, 62);
    Ego ^= Do;
    BCe = ROL(Ego, 55);
    Eku ^= Du;
    BCi = ROL(Eku, 39);
    Ema ^= Da;
    BCo = ROL(Ema, 41);
    Ese ^= De;
    BCu = ROL(Ese, 2);
    Asa = BCa ^ ((~BCe) & BCi);
    Ase = BCe ^ ((~BCi) & BCo);
    Asi = BCi ^ ((~BCo) & BCu);
    Aso = BCo ^ ((~BCu) & BCa);
    Asu = BCu ^ ((~BCa) & BCe);
  }

  // copyToState(state, A)
  state[0] = Aba;
  state[1] = Abe;
  state[2] = Abi;
  state[3] = Abo;
  state[4] = Abu;
  state[5] = Aga;
  state[6] = Age;
  state[7] = Agi;
  state[8] = Ago;
  state[9] = Agu;
  state[10] = Aka;
  state[11] = Ake;
  state[12] = Aki;
  state[13] = Ako;
  state[14] = Aku;
  state[15] = Ama;
  state[16] = Ame;
  state[17] = Ami;
  state[18] = Amo;
  state[19] = Amu;
  state[20] = Asa;
  state[21] = Ase;
  state[22] = Asi;
  state[23] = Aso;
  state[24] = Asu;
}

/**
 * @brief Initialize masked incremental Keccak context to zero.
 *
 * @param[out] [ctx] Pointer to masked incremental Keccak context
 */
static void MSK_keccak_inc_init(MSK_shake256incctx *ctx) {
  memset(ctx->state, 0, 25 * N_SHARES * sizeof(uint64_t));
  ctx->nsqueezed = 0;
}

void MSK_shake256_inc_init(MSK_shake256incctx *ctx) {
  MSK_keccak_inc_init(ctx);
}

/**
 * @brief Initialize incremental Keccak context to zero.
 *
 * @param[out] ctx Pointer to incremental Keccak context
 */
static void keccak_inc_init(shake256incctx *ctx) {
  memset(ctx->state, 0, 25 * sizeof(uint64_t));
  ctx->nsqueezed = 0;
}

void shake256_inc_init(shake256incctx *ctx) { keccak_inc_init(ctx); }

/**
 * @brief Incremental masked Keccak absorb.
 *
 * Preceded by keccak_inc_init, succeeded by keccak_inc_finalize
 *
 * @param[inout] [ctx] Pointer to masked incremental context
 * @param[in] r rate in bytes (e.g., 168 for SHAKE128)
 * @param[in] [m] Array of mlen N_SHARES-shares boolean-masked bytes to absorb
 * @param[in] mlen Byte-length of m input
 */
static void MSK_keccak_inc_absorb(MSK_shake256incctx *ctx, const size_t r,
                                  const MSK_uint8_t *m, size_t mlen) {
  size_t s_byte;

  /* ctx->nsqueezed is the number of non-absorbed bytes xored into the state */
  while (mlen + ctx->nsqueezed >= r) {
    for (size_t i = 0; i < r - (uint32_t)ctx->nsqueezed; i++) {
      /* Take the i'th byte from message
         xor with the ctx->nsqueezed + i'th byte of the state; little-endian */
      s_byte = (ctx->nsqueezed + i) >> 3;
      for (uint8_t l = 0; l < N_SHARES; l++) {
        ctx->state[s_byte][l] ^= (uint64_t)m[i][l]
                                 << (8 * ((ctx->nsqueezed + i) & 0x07));
      }
    }

    mlen -= (size_t)(r - ctx->nsqueezed);
    m += r - ctx->nsqueezed;
    ctx->nsqueezed = 0;

    MSK_KeccakF1600_StatePermute(ctx->state);
  }

  for (size_t i = 0; i < mlen; i++) {
    s_byte = (ctx->nsqueezed + i) >> 3;
    for (size_t l = 0; l < N_SHARES; l++) {
      ctx->state[s_byte][l] ^= (uint64_t)m[i][l]
                               << (8 * ((ctx->nsqueezed + i) & 0x07));
    }
  }
  ctx->nsqueezed += mlen;
}

void MSK_shake256_inc_absorb(MSK_shake256incctx *ctx, const MSK_uint8_t *input,
                             const size_t inlen) {
  MSK_keccak_inc_absorb(ctx, SHAKE256_RATE, input, inlen);
}

static void keccak_inc_absorb(shake256incctx *ctx, uint32_t r, const uint8_t *m,
                              size_t mlen) {
  size_t i;

  /* that ctx->nsqueezed is the number of non-absorbed bytes xored into the
   * state */
  while (mlen + ctx->nsqueezed >= r) {
    for (i = 0; i < r - (uint32_t)ctx->nsqueezed; i++) {
      /* Take the i'th byte from message
         xor with the ctx->nsqueezed + i'th byte of the state; little-endian */
      ctx->state[(ctx->nsqueezed + i) >> 3] ^=
          (uint64_t)m[i] << (8 * ((ctx->nsqueezed + i) & 0x07));
    }

    mlen -= (size_t)(r - ctx->nsqueezed);
    m += r - ctx->nsqueezed;
    ctx->nsqueezed = 0;

    KeccakF1600_StatePermute(ctx->state);
  }

  for (i = 0; i < mlen; i++) {
    ctx->state[(ctx->nsqueezed + i) >> 3] ^=
        (uint64_t)m[i] << (8 * ((ctx->nsqueezed + i) & 0x07));
  }
  ctx->nsqueezed += mlen;
}

void shake256_inc_absorb(shake256incctx *ctx, const uint8_t *input,
                         const size_t inlen) {
  keccak_inc_absorb(ctx, SHAKE256_RATE, input, inlen);
}

/**
 * @brief Incremental Keccak masked finalization
 *
 * @param[inout] [ctx] Pointer to incremental SHAKE-256 masked context
 * @param[in] r Rate in bytes (e.g., 168 for SHAKE128)
 * @param[in] p Domain separation byte
 */
static void MSK_keccak_inc_finalize(MSK_shake256incctx *ctx, const size_t r,
                                    const uint8_t p) {
  /* After keccak_inc_absorb, we are guaranteed that ctx->nsqueezed < r,
     so we can always use one more byte for p in the current state. */
  ctx->state[ctx->nsqueezed >> 3][0] ^= (uint64_t)p
                                        << (8 * (ctx->nsqueezed & 0x07));
  ctx->state[(r - 1) >> 3][0] ^= (uint64_t)128 << (8 * ((r - 1) & 0x07));
  ctx->nsqueezed = 0;
}

void MSK_shake256_inc_finalize(MSK_shake256incctx *ctx) {
  MSK_keccak_inc_finalize(ctx, SHAKE256_RATE, 0x1F);
}

/**
 * @brief Incremental Keccak finalization
 *
 * @param[inout] ctx Pointer to incremental SHAKE-256 context
 * @param[in] r Rate in bytes (e.g., 168 for SHAKE128)
 * @param[in] p Domain separation byte
 */
static void keccak_inc_finalize(shake256incctx *ctx, size_t r, uint8_t p) {
  /* After keccak_inc_absorb, we are guaranteed that ctx->nsqueezed < r,
     so we can always use one more byte for p in the current state. */
  ctx->state[ctx->nsqueezed >> 3] ^= (uint64_t)p
                                     << (8 * (ctx->nsqueezed & 0x07));
  ctx->state[(r - 1) >> 3] ^= (uint64_t)128 << (8 * ((r - 1) & 0x07));
  ctx->nsqueezed = 0;
}

void shake256_inc_finalize(shake256incctx *ctx) {
  keccak_inc_finalize(ctx, SHAKE256_RATE, 0x1F);
}

/**
 * @brief Incremental masked Keccak squeeze; can be called on byte-level
 *
 * @param[out] [h] Array of outlen N_SHARES-shares boolean-masked bytes
 * @param[inout] [ctx] pointer to input/output incremental masked context
 * @param[in] outlen number of bytes to be squeezed
 * @param[in] r Rate in bytes (e.g., 168 for SHAKE128)
 */
static void MSK_keccak_inc_squeeze(MSK_uint8_t h[], MSK_shake256incctx *ctx,
                                   size_t outlen, const size_t r) {

  size_t i;
  MSK_uint8_t(*new_h);

  /* First consume any bytes we still have sitting around */
  for (i = 0; i < outlen && i < ctx->nsqueezed; i++) {
    for (uint8_t l = 0; l < N_SHARES; l++) {
      /* There are ctx->nsqueezed bytes left, so r - ctx->nsqueezed is the first
      available byte. We consume from there, i.e., up to r. */
      h[i][l] = (uint8_t)(ctx->state[(r - ctx->nsqueezed + i) >> 3][l] >>
                          (8 * ((r - ctx->nsqueezed + i) & 0x07)));
    }
  }
  new_h = &h[i];
  outlen -= i;
  ctx->nsqueezed -= i;

  /* Then squeeze the remaining necessary blocks */
  while (outlen > 0) {
    MSK_KeccakF1600_StatePermute(ctx->state);
    for (i = 0; i < outlen && i < r; i++) {
      for (uint8_t l = 0; l < N_SHARES; l++) {
        new_h[i][l] = (uint8_t)(ctx->state[i >> 3][l] >> (8 * (i & 0x07)));
      }
    }
    new_h = &new_h[i];
    outlen -= i;
    ctx->nsqueezed = r - i;
  }
}

void MSK_shake256_inc_squeeze(MSK_uint8_t output[], MSK_shake256incctx *ctx,
                              const size_t outlen) {
  MSK_keccak_inc_squeeze(output, ctx, outlen, SHAKE256_RATE);
}

/**
 * @brief Incremental Keccak squeeze; can be called on byte-level
 *
 * @param[out] h Array of outlen bytes
 * @param[inout] ctx pointer to input/output incremental context
 * @param[in] outlen number of bytes to be squeezed
 * @param[in] r Rate in bytes (e.g., 168 for SHAKE128)
 */
static void keccak_inc_squeeze(uint8_t *h, shake256incctx *ctx, size_t outlen,
                               uint32_t r) {
  size_t i;

  /* First consume any bytes we still have sitting around */
  for (i = 0; i < outlen && i < ctx->nsqueezed; i++) {
    /* There are ctx->nsqueezed bytes left, so r - ctx->nsqueezed is the first
       available byte. We consume from there, i.e., up to r. */
    h[i] = (uint8_t)(ctx->state[(r - ctx->nsqueezed + i) >> 3] >>
                     (8 * ((r - ctx->nsqueezed + i) & 0x07)));
  }
  h += i;
  outlen -= i;
  ctx->nsqueezed -= i;

  /* Then squeeze the remaining necessary blocks */
  while (outlen > 0) {
    KeccakF1600_StatePermute(ctx->state);

    for (i = 0; i < outlen && i < r; i++) {
      h[i] = (uint8_t)(ctx->state[i >> 3] >> (8 * (i & 0x07)));
    }
    h += i;
    outlen -= i;
    ctx->nsqueezed = r - i;
  }
}

void shake256_inc_squeeze(uint8_t *output, shake256incctx *ctx,
                          const size_t outlen) {
  keccak_inc_squeeze(output, ctx, outlen, SHAKE256_RATE);
}
