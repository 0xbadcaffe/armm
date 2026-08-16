#ifndef ARMM_FEATURES_H
#define ARMM_FEATURES_H
#include "armm_intrinsics.h"

typedef enum {
    ARMM_ARCH_UNKNOWN = 0,
    ARMM_ARCH_ARMV6_M,
    ARMM_ARCH_ARMV7_M,
    ARMM_ARCH_ARMV7E_M,
    ARMM_ARCH_ARMV8_M_BASELINE,
    ARMM_ARCH_ARMV8_M_MAINLINE,
    ARMM_ARCH_ARMV81_M_MAINLINE
} armm_architecture_t;

ARMM_ALWAYS_INLINE armm_architecture_t armm_architecture(void)
{
#if ARMM_ARCH_V6M
    return ARMM_ARCH_ARMV6_M;
#elif ARMM_ARCH_V7M
    return ARMM_ARCH_ARMV7_M;
#elif ARMM_ARCH_V7EM
    return ARMM_ARCH_ARMV7E_M;
#elif ARMM_ARCH_V8M_BASE
    return ARMM_ARCH_ARMV8_M_BASELINE;
#elif ARMM_ARCH_V8M_MAIN
    return ARMM_ARCH_ARMV8_M_MAINLINE;
#elif ARMM_ARCH_V81M_MAIN
    return ARMM_ARCH_ARMV81_M_MAINLINE;
#else
    return ARMM_ARCH_UNKNOWN;
#endif
}
ARMM_ALWAYS_INLINE bool armm_has_fpu(void) { return ARMM_HAS_FPU != 0; }
ARMM_ALWAYS_INLINE bool armm_has_dsp(void) { return ARMM_HAS_DSP != 0; }
ARMM_ALWAYS_INLINE bool armm_has_mve(void) { return ARMM_HAS_MVE != 0; }
ARMM_ALWAYS_INLINE bool armm_has_trustzone(void) { return ARMM_HAS_TRUSTZONE != 0; }
ARMM_ALWAYS_INLINE bool armm_secure_build(void) { return ARMM_SECURE_BUILD != 0; }
#endif
