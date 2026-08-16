#ifndef ARMM_MPU_H
#define ARMM_MPU_H
#include "armm_intrinsics.h"

#if ARMM_HAS_MPU
#define ARMM_MPU_BASE      (ARMM_SCS_BASE + 0x0D90UL)
#define ARMM_MPU_TYPE      ARMM_REG32(ARMM_MPU_BASE + 0x00UL)
#define ARMM_MPU_CTRL      ARMM_REG32(ARMM_MPU_BASE + 0x04UL)
#define ARMM_MPU_RNR       ARMM_REG32(ARMM_MPU_BASE + 0x08UL)
#define ARMM_MPU_RBAR      ARMM_REG32(ARMM_MPU_BASE + 0x0CUL)
#define ARMM_MPU_RASR      ARMM_REG32(ARMM_MPU_BASE + 0x10UL)
#define ARMM_MPU_RLAR      ARMM_REG32(ARMM_MPU_BASE + 0x10UL)
#define ARMM_MPU_MAIR0     ARMM_REG32(ARMM_MPU_BASE + 0x30UL)
#define ARMM_MPU_MAIR1     ARMM_REG32(ARMM_MPU_BASE + 0x34UL)
#define ARMM_MPU_CTRL_ENABLE (1UL << 0)
#define ARMM_MPU_CTRL_HFNMIENA (1UL << 1)
#define ARMM_MPU_CTRL_PRIVDEFENA (1UL << 2)

ARMM_ALWAYS_INLINE uint32_t armm_mpu_region_count(void) { return (ARMM_MPU_TYPE >> 8) & 0xFFu; }
ARMM_ALWAYS_INLINE void armm_mpu_disable(void) { armm_dmb(); ARMM_MPU_CTRL = 0u; armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_mpu_enable(bool privileged_default_map, bool hardfault_nmi)
{
    uint32_t v = ARMM_MPU_CTRL_ENABLE;
    if (privileged_default_map) v |= ARMM_MPU_CTRL_PRIVDEFENA;
    if (hardfault_nmi) v |= ARMM_MPU_CTRL_HFNMIENA;
    ARMM_MPU_CTRL = v; armm_dsb(); armm_isb();
}

#if ARMM_ARCH_V7M || ARMM_ARCH_V7EM
/* Caller supplies architecturally encoded RBAR/RASR values. */
ARMM_ALWAYS_INLINE void armm_mpu_v7_region_set(uint32_t region, uint32_t rbar, uint32_t rasr)
{
    ARMM_MPU_RNR = region; ARMM_MPU_RBAR = rbar; ARMM_MPU_RASR = rasr; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_mpu_region_disable(uint32_t region)
{
    ARMM_MPU_RNR = region; ARMM_MPU_RASR = 0u; armm_dsb(); armm_isb();
}
#endif

#if ARMM_ARCH_V8PLUS
/* Armv8-M/Armv8.1-M MPU uses RBAR/RLAR and MAIR attributes. */
ARMM_ALWAYS_INLINE void armm_mpu_mair_set(uint32_t index, uint8_t attr)
{
    volatile uint32_t *reg = (index < 4u) ? (volatile uint32_t *)(uintptr_t)(ARMM_MPU_BASE + 0x30UL)
                                          : (volatile uint32_t *)(uintptr_t)(ARMM_MPU_BASE + 0x34UL);
    uint32_t shift = (index & 3u) * 8u;
    uint32_t v = *reg; v = (v & ~(0xFFu << shift)) | ((uint32_t)attr << shift); *reg = v;
}
ARMM_ALWAYS_INLINE void armm_mpu_v8_region_set(uint32_t region, uint32_t rbar, uint32_t rlar)
{
    ARMM_MPU_RNR = region; ARMM_MPU_RBAR = rbar; ARMM_MPU_RLAR = rlar; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_mpu_region_disable(uint32_t region)
{
    ARMM_MPU_RNR = region; ARMM_MPU_RLAR &= ~1UL; armm_dsb(); armm_isb();
}
#endif
#endif
#endif
