#ifndef ARMM_SYSTICK_H
#define ARMM_SYSTICK_H
#include "armm_intrinsics.h"

#define ARMM_SYSTICK_BASE   (ARMM_SCS_BASE + 0x0010UL)
#define ARMM_SYSTICK_CTRL   ARMM_REG32(ARMM_SYSTICK_BASE + 0x0UL)
#define ARMM_SYSTICK_LOAD   ARMM_REG32(ARMM_SYSTICK_BASE + 0x4UL)
#define ARMM_SYSTICK_VAL    ARMM_REG32(ARMM_SYSTICK_BASE + 0x8UL)
#define ARMM_SYSTICK_CALIB  ARMM_REG32(ARMM_SYSTICK_BASE + 0xCUL)
#define ARMM_SYSTICK_ENABLE (1UL << 0)
#define ARMM_SYSTICK_TICKINT (1UL << 1)
#define ARMM_SYSTICK_CLKSOURCE (1UL << 2)
#define ARMM_SYSTICK_COUNTFLAG (1UL << 16)

ARMM_ALWAYS_INLINE void armm_systick_stop(void) { ARMM_SYSTICK_CTRL = 0u; }
ARMM_ALWAYS_INLINE bool armm_systick_config(uint32_t ticks, bool interrupt_enable)
{
    if (ticks == 0u || (ticks - 1u) > 0xFFFFFFu) return false;
    ARMM_SYSTICK_LOAD = ticks - 1u;
    ARMM_SYSTICK_VAL = 0u;
    ARMM_SYSTICK_CTRL = ARMM_SYSTICK_ENABLE | ARMM_SYSTICK_CLKSOURCE |
                        (interrupt_enable ? ARMM_SYSTICK_TICKINT : 0u);
    return true;
}
ARMM_ALWAYS_INLINE uint32_t armm_systick_value(void) { return ARMM_SYSTICK_VAL & 0xFFFFFFu; }
ARMM_ALWAYS_INLINE uint32_t armm_systick_reload(void) { return ARMM_SYSTICK_LOAD & 0xFFFFFFu; }
ARMM_ALWAYS_INLINE bool armm_systick_wrapped(void) { return (ARMM_SYSTICK_CTRL & ARMM_SYSTICK_COUNTFLAG) != 0u; }
ARMM_ALWAYS_INLINE uint32_t armm_systick_calibration(void) { return ARMM_SYSTICK_CALIB; }
#endif
