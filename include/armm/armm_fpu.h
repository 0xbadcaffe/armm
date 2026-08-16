#ifndef ARMM_FPU_H
#define ARMM_FPU_H
#include "armm_scb.h"

#if ARMM_HAS_FPU
#define ARMM_FPU_BASE  (ARMM_SCS_BASE + 0x0F30UL)
#define ARMM_FPU_FPCCR ARMM_REG32(ARMM_FPU_BASE + 0x04UL)
#define ARMM_FPU_FPCAR ARMM_REG32(ARMM_FPU_BASE + 0x08UL)
#define ARMM_FPU_FPDSCR ARMM_REG32(ARMM_FPU_BASE + 0x0CUL)
#define ARMM_FPU_MVFR0 ARMM_REG32(ARMM_FPU_BASE + 0x10UL)
#define ARMM_FPU_MVFR1 ARMM_REG32(ARMM_FPU_BASE + 0x14UL)
#define ARMM_FPU_MVFR2 ARMM_REG32(ARMM_FPU_BASE + 0x18UL)
#define ARMM_FPCCR_ASPEN (1UL << 31)
#define ARMM_FPCCR_LSPEN (1UL << 30)

ARMM_ALWAYS_INLINE void armm_fpu_enable(void)
{
    ARMM_SCB_CPACR |= (0xFu << 20); armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_fpu_disable(void)
{
    ARMM_SCB_CPACR &= ~(0xFu << 20); armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE bool armm_fpu_is_enabled(void) { return (ARMM_SCB_CPACR & (0xFu << 20)) == (0xFu << 20); }
ARMM_ALWAYS_INLINE void armm_fpu_lazy_stacking_enable(void) { ARMM_FPU_FPCCR |= ARMM_FPCCR_ASPEN | ARMM_FPCCR_LSPEN; }
ARMM_ALWAYS_INLINE void armm_fpu_lazy_stacking_disable(void) { ARMM_FPU_FPCCR &= ~(ARMM_FPCCR_ASPEN | ARMM_FPCCR_LSPEN); }
#endif
#endif
