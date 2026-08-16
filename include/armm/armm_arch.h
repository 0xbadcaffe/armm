#ifndef ARMM_ARCH_H
#define ARMM_ARCH_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Architecture detection. All ARMM_HAS_* macros may be overridden before
 * including armm.h when a particular core implementation omits/adds an
 * optional architectural feature.
 */

#if defined(__ARM_ARCH_6M__)
# define ARMM_ARCH_V6M 1
#else
# define ARMM_ARCH_V6M 0
#endif

#if defined(__ARM_ARCH_7M__)
# define ARMM_ARCH_V7M 1
#else
# define ARMM_ARCH_V7M 0
#endif

#if defined(__ARM_ARCH_7EM__)
# define ARMM_ARCH_V7EM 1
#else
# define ARMM_ARCH_V7EM 0
#endif

#if defined(__ARM_ARCH_8M_BASE__)
# define ARMM_ARCH_V8M_BASE 1
#else
# define ARMM_ARCH_V8M_BASE 0
#endif

#if defined(__ARM_ARCH_8M_MAIN__)
# define ARMM_ARCH_V8M_MAIN 1
#else
# define ARMM_ARCH_V8M_MAIN 0
#endif

#if defined(__ARM_ARCH_8_1M_MAIN__)
# define ARMM_ARCH_V81M_MAIN 1
#else
# define ARMM_ARCH_V81M_MAIN 0
#endif

#define ARMM_ARCH_MAINLINE (ARMM_ARCH_V7M || ARMM_ARCH_V7EM || ARMM_ARCH_V8M_MAIN || ARMM_ARCH_V81M_MAIN)
#define ARMM_ARCH_V8PLUS   (ARMM_ARCH_V8M_BASE || ARMM_ARCH_V8M_MAIN || ARMM_ARCH_V81M_MAIN)

#ifndef ARMM_HAS_BASEPRI
# define ARMM_HAS_BASEPRI ARMM_ARCH_MAINLINE
#endif
#ifndef ARMM_HAS_FAULTMASK
# define ARMM_HAS_FAULTMASK ARMM_ARCH_MAINLINE
#endif
#ifndef ARMM_HAS_CFSR
# define ARMM_HAS_CFSR ARMM_ARCH_MAINLINE
#endif
#ifndef ARMM_HAS_DWT
# define ARMM_HAS_DWT (!ARMM_ARCH_V6M && !ARMM_ARCH_V8M_BASE)
#endif
#ifndef ARMM_HAS_MPU
# if defined(__MPU_PRESENT)
#  define ARMM_HAS_MPU (__MPU_PRESENT + 0)
# else
#  define ARMM_HAS_MPU 0
# endif
#endif
#ifndef ARMM_HAS_FPU
# if defined(__ARM_FP) && (__ARM_FP != 0)
#  define ARMM_HAS_FPU 1
# else
#  define ARMM_HAS_FPU 0
# endif
#endif
#ifndef ARMM_HAS_DSP
# if defined(__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1)
#  define ARMM_HAS_DSP 1
# else
#  define ARMM_HAS_DSP 0
# endif
#endif
#ifndef ARMM_HAS_MVE
# if defined(__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE != 0)
#  define ARMM_HAS_MVE 1
# else
#  define ARMM_HAS_MVE 0
# endif
#endif
#ifndef ARMM_HAS_TRUSTZONE
# if defined(__ARM_FEATURE_CMSE) && ((__ARM_FEATURE_CMSE & 1) != 0)
#  define ARMM_HAS_TRUSTZONE 1
# else
#  define ARMM_HAS_TRUSTZONE 0
# endif
#endif
#ifndef ARMM_SECURE_BUILD
# if defined(__ARM_FEATURE_CMSE) && ((__ARM_FEATURE_CMSE & 2) != 0)
#  define ARMM_SECURE_BUILD 1
# else
#  define ARMM_SECURE_BUILD 0
# endif
#endif
#ifndef ARMM_HAS_SAU
# define ARMM_HAS_SAU ARMM_SECURE_BUILD
#endif
#ifndef ARMM_HAS_STACK_LIMIT
# define ARMM_HAS_STACK_LIMIT (ARMM_ARCH_V8M_MAIN || ARMM_ARCH_V81M_MAIN)
#endif
#ifndef ARMM_HAS_VTOR
# define ARMM_HAS_VTOR (!ARMM_ARCH_V6M)
#endif

#ifndef ARMM_CFG_NVIC_PRIO_BITS
#define ARMM_CFG_NVIC_PRIO_BITS 3u
#endif

#define ARMM_SCS_BASE       0xE000E000UL
#define ARMM_SCS_NS_BASE    0xE002E000UL
#define ARMM_ITM_BASE       0xE0000000UL
#define ARMM_DWT_BASE       0xE0001000UL
#define ARMM_COREDEBUG_BASE 0xE000EDF0UL

#define ARMM_REG32(addr_) (*(volatile uint32_t *)(uintptr_t)(addr_))
#define ARMM_REG8(addr_)  (*(volatile uint8_t  *)(uintptr_t)(addr_))

#endif
