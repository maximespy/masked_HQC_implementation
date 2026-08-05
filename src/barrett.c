#include "barrett.h"
#include "parameters.h"

#if SECURITY_LEVEL == 128

static const uint32_t m_val[PARAM_OMEGA_E] = {
    243079, 243093, 243106, 243120, 243134, 243148, 243161, 243175, 243189,
    243203, 243216, 243230, 243244, 243258, 243272, 243285, 243299, 243313,
    243327, 243340, 243354, 243368, 243382, 243396, 243409, 243423, 243437,
    243451, 243465, 243478, 243492, 243506, 243520, 243534, 243547, 243561,
    243575, 243589, 243603, 243616, 243630, 243644, 243658, 243672, 243686,
    243699, 243713, 243727, 243741, 243755, 243769, 243782, 243796, 243810,
    243824, 243838, 243852, 243865, 243879, 243893, 243907, 243921, 243935,
    243949, 243962, 243976, 243990, 244004, 244018, 244032, 244046, 244059,
    244073, 244087, 244101};

#elif SECURITY_LEVEL == 192

static const uint32_t m_val[PARAM_OMEGA_E] = {
    119800, 119803, 119807, 119810, 119813, 119817, 119820, 119823, 119827,
    119830, 119833, 119837, 119840, 119843, 119847, 119850, 119853, 119857,
    119860, 119864, 119867, 119870, 119874, 119877, 119880, 119884, 119887,
    119890, 119894, 119897, 119900, 119904, 119907, 119910, 119914, 119917,
    119920, 119924, 119927, 119930, 119934, 119937, 119941, 119944, 119947,
    119951, 119954, 119957, 119961, 119964, 119967, 119971, 119974, 119977,
    119981, 119984, 119987, 119991, 119994, 119997, 120001, 120004, 120008,
    120011, 120014, 120018, 120021, 120024, 120028, 120031, 120034, 120038,
    120041, 120044, 120048, 120051, 120054, 120058, 120061, 120065, 120068,
    120071, 120075, 120078, 120081, 120085, 120088, 120091, 120095, 120098,
    120101, 120105, 120108, 120112, 120115, 120118, 120122, 120125, 120128,
    120132, 120135, 120138, 120142, 120145, 120149, 120152, 120155, 120159,
    120162, 120165, 120169, 120172, 120175, 120179};

#elif SECURITY_LEVEL == 256

static const uint32_t m_val[PARAM_OMEGA_E] = {
    74517, 74518, 74520, 74521, 74522, 74524, 74525, 74526, 74527, 74529, 74530,
    74531, 74533, 74534, 74535, 74536, 74538, 74539, 74540, 74542, 74543, 74544,
    74545, 74547, 74548, 74549, 74551, 74552, 74553, 74555, 74556, 74557, 74558,
    74560, 74561, 74562, 74564, 74565, 74566, 74567, 74569, 74570, 74571, 74573,
    74574, 74575, 74577, 74578, 74579, 74580, 74582, 74583, 74584, 74586, 74587,
    74588, 74590, 74591, 74592, 74593, 74595, 74596, 74597, 74599, 74600, 74601,
    74602, 74604, 74605, 74606, 74608, 74609, 74610, 74612, 74613, 74614, 74615,
    74617, 74618, 74619, 74621, 74622, 74623, 74625, 74626, 74627, 74628, 74630,
    74631, 74632, 74634, 74635, 74636, 74637, 74639, 74640, 74641, 74643, 74644,
    74645, 74647, 74648, 74649, 74650, 74652, 74653, 74654, 74656, 74657, 74658,
    74660, 74661, 74662, 74663, 74665, 74666, 74667, 74669, 74670, 74671, 74673,
    74674, 74675, 74676, 74678, 74679, 74680, 74682, 74683, 74684, 74685, 74687,
    74688, 74689, 74691, 74692, 74693, 74695, 74696, 74697, 74698, 74700, 74701,
    74702, 74704, 74705, 74706, 74708, 74709};

#else

#error "Invalid security level"

#endif

/**
 * @brief 64-bits N_SHARES-shares share-wise product with a 32-bits scalar
 * @param[out] [x] Result (x[i] = a[i] * b), 64-bits N_SHARES-shares boolean
 * value
 * @param[in] [a] First operand, 64-bits N_SHARES-shares boolean value
 * @param[in] b Second operand, 32-bits value
 */
void MSK_64b_UNMSK_32b_mul(MSK_uint64_t x, const MSK_uint64_t a,
                           const uint32_t m) {
  for (size_t i = 0; i < N_SHARES; i++) {
    x[i] = a[i] * m;
  }
}

/**
 * @brief 64-bits N_SHARES-shares share-wise subtraction
 * @param[out] [x] Result (x[i] = a[i] - b[i]), 64-bits N_SHARES-shares boolean
 * value
 * @param[in] [a] First operand, 64-bits N_SHARES-shares boolean value
 * @param[in] [b] Second operand, 64-bits N_SHARES-shares boolean value
 */
void MSK_64b_sharewise_sub(MSK_uint64_t x, const MSK_uint64_t a,
                           const MSK_uint64_t b) {
  for (size_t i = 0; i < N_SHARES; i++) {
    x[i] = a[i] - b[i];
  }
}

void MSK_barrett(MSK_uint32_t x, MSK_uint32_t a, const uint8_t i) {
  MSK_uint64_t q = {0};
  MSK_uint64_t a_qn = {0};
  MSK_uint32_t a_qn32 = {0};
  MSK_uint32_t A = {0};
  MSK_uint32_t z = {0};
  MSK_uint64_t ari_a = {0};
  MSK_uint64_t ari_q = {0};
  MSK_uint64_t ari_qn = {0};
  MSK_uint64_t ari_a_qn = {0};
  MSK_uint64_t a64 = {0};
  uint32_t n;

  // Actual modulus
  n = PARAM_N - i;

  // Cut path between inputs and outputs
  MSK_SNI_32b_inplace_refresh(a);

  // q = (a * m) >> k
  MSK_32b_to_64b_copy(a64, a);
  MSK_boolean_to_arithmetic(ari_a, a64, N_SHARES);
  MSK_64b_UNMSK_32b_mul(ari_q, ari_a, m_val[i]);

  MSK_arithmetic_to_boolean(q, ari_q, N_SHARES);

  MSK_64b_right_shift(q, q, 32);

  MSK_boolean_to_arithmetic(ari_q, q, N_SHARES);

  // q * n
  MSK_64b_UNMSK_32b_mul(ari_qn, ari_q, n);

  // a - q * n
  MSKA_SNI_64b_inplace_refresh(ari_a);
  MSK_64b_sharewise_sub(ari_a_qn, ari_a, ari_qn);
  MSK_arithmetic_to_boolean(a_qn, ari_a_qn, N_SHARES);

  MSK_64b_to_32b_copy(a_qn32, a_qn);

  // Conditional subtraction
  MSK_32b_semi_sub(A, a_qn32, n);

  // z = sign of A
  MSK_32b_right_shift(z, A, 31);

  MSK_SNI_32b_inplace_refresh(a_qn32);
  MSK_SNI_32b_inplace_refresh(A);

  // Final subtraction needed if z = 1
  MSK_32b_if(x, A, a_qn32, z);
}
