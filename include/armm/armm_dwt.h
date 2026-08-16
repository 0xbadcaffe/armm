#ifndef ARMM_DWT_H
#define ARMM_DWT_H
#include "armm_intrinsics.h"

#if ARMM_HAS_DWT
#define ARMM_DWT_CTRL        ARMM_REG32(ARMM_DWT_BASE + 0x000UL)
#define ARMM_DWT_CYCCNT      ARMM_REG32(ARMM_DWT_BASE + 0x004UL)
#define ARMM_DWT_CPICNT      ARMM_REG32(ARMM_DWT_BASE + 0x008UL)
#define ARMM_DWT_EXCCNT      ARMM_REG32(ARMM_DWT_BASE + 0x00CUL)
#define ARMM_DWT_SLEEPCNT    ARMM_REG32(ARMM_DWT_BASE + 0x010UL)
#define ARMM_DWT_LSUCNT      ARMM_REG32(ARMM_DWT_BASE + 0x014UL)
#define ARMM_DWT_FOLDCNT     ARMM_REG32(ARMM_DWT_BASE + 0x018UL)
#define ARMM_DWT_COMP(n_)    ARMM_REG32(ARMM_DWT_BASE + 0x020UL + 0x10UL*(n_))
#define ARMM_DWT_MASK(n_)    ARMM_REG32(ARMM_DWT_BASE + 0x024UL + 0x10UL*(n_))
#define ARMM_DWT_FUNCTION(n_) ARMM_REG32(ARMM_DWT_BASE + 0x028UL + 0x10UL*(n_))
#define ARMM_DEMCR           ARMM_REG32(ARMM_COREDEBUG_BASE + 0x0CUL)
#define ARMM_DEMCR_TRCENA    (1UL << 24)
#define ARMM_DWT_CYCCNTENA   (1UL << 0)

ARMM_ALWAYS_INLINE bool armm_dwt_cycle_counter_enable(void)
{
    ARMM_DEMCR |= ARMM_DEMCR_TRCENA;
    ARMM_DWT_CYCCNT = 0u;
    ARMM_DWT_CTRL |= ARMM_DWT_CYCCNTENA;
    armm_isb();
    return (ARMM_DWT_CTRL & ARMM_DWT_CYCCNTENA) != 0u;
}
ARMM_ALWAYS_INLINE void armm_dwt_cycle_counter_disable(void) { ARMM_DWT_CTRL &= ~ARMM_DWT_CYCCNTENA; }
ARMM_ALWAYS_INLINE uint32_t armm_cycles(void) { return ARMM_DWT_CYCCNT; }
ARMM_ALWAYS_INLINE uint32_t armm_cycles_elapsed(uint32_t start) { return ARMM_DWT_CYCCNT - start; }
ARMM_ALWAYS_INLINE uint32_t armm_dwt_comparator_count(void) { return (ARMM_DWT_CTRL >> 28) & 0xFu; }
ARMM_ALWAYS_INLINE uint32_t armm_us_to_cycles(uint32_t us, uint32_t hz) { return (uint32_t)(((uint64_t)us * hz) / 1000000ULL); }
ARMM_ALWAYS_INLINE uint32_t armm_ms_to_cycles(uint32_t ms, uint32_t hz) { return (uint32_t)(((uint64_t)ms * hz) / 1000ULL); }
ARMM_ALWAYS_INLINE void armm_delay_cycles(uint32_t cycles) { uint32_t s = armm_cycles(); while (armm_cycles_elapsed(s) < cycles) armm_nop(); }
ARMM_ALWAYS_INLINE void armm_delay_us(uint32_t us, uint32_t hz) { armm_delay_cycles(armm_us_to_cycles(us, hz)); }
ARMM_ALWAYS_INLINE void armm_delay_ms(uint32_t ms, uint32_t hz)
{
    while (ms--) armm_delay_cycles(hz / 1000u);
}
#endif
#endif
