#ifndef ARMM_SCB_H
#define ARMM_SCB_H

#include "armm_intrinsics.h"

#define ARMM_SCB_BASE       (ARMM_SCS_BASE + 0x0D00UL)
#define ARMM_SCB_CPUID      ARMM_REG32(ARMM_SCB_BASE + 0x000UL)
#define ARMM_SCB_ICSR       ARMM_REG32(ARMM_SCB_BASE + 0x004UL)
#define ARMM_SCB_VTOR       ARMM_REG32(ARMM_SCB_BASE + 0x008UL)
#define ARMM_SCB_AIRCR      ARMM_REG32(ARMM_SCB_BASE + 0x00CUL)
#define ARMM_SCB_SCR        ARMM_REG32(ARMM_SCB_BASE + 0x010UL)
#define ARMM_SCB_CCR        ARMM_REG32(ARMM_SCB_BASE + 0x014UL)
#define ARMM_SCB_SHPR(n_)   ARMM_REG8 (ARMM_SCB_BASE + 0x018UL + (n_))
#define ARMM_SCB_SHCSR      ARMM_REG32(ARMM_SCB_BASE + 0x024UL)
#define ARMM_SCB_CFSR       ARMM_REG32(ARMM_SCB_BASE + 0x028UL)
#define ARMM_SCB_HFSR       ARMM_REG32(ARMM_SCB_BASE + 0x02CUL)
#define ARMM_SCB_DFSR       ARMM_REG32(ARMM_SCB_BASE + 0x030UL)
#define ARMM_SCB_MMFAR      ARMM_REG32(ARMM_SCB_BASE + 0x034UL)
#define ARMM_SCB_BFAR       ARMM_REG32(ARMM_SCB_BASE + 0x038UL)
#define ARMM_SCB_AFSR       ARMM_REG32(ARMM_SCB_BASE + 0x03CUL)
#define ARMM_SCB_CPACR      ARMM_REG32(ARMM_SCB_BASE + 0x088UL)

#define ARMM_ICSR_PENDSVSET (1UL << 28)
#define ARMM_ICSR_PENDSVCLR (1UL << 27)
#define ARMM_ICSR_PENDSTSET (1UL << 26)
#define ARMM_ICSR_PENDSTCLR (1UL << 25)
#define ARMM_SCR_SLEEPDEEP  (1UL << 2)
#define ARMM_SCR_SLEEPONEXIT (1UL << 1)
#define ARMM_AIRCR_VECTKEY  (0x5FAUL << 16)
#define ARMM_AIRCR_SYSRESETREQ (1UL << 2)

ARMM_ALWAYS_INLINE uint32_t armm_cpuid(void) { return ARMM_SCB_CPUID; }
ARMM_ALWAYS_INLINE uint32_t armm_priority_grouping_get(void) { return (ARMM_SCB_AIRCR >> 8) & 7u; }
ARMM_ALWAYS_INLINE void armm_priority_grouping_set(uint32_t group)
{
    uint32_t v = ARMM_SCB_AIRCR;
    v &= ~((0xFFFFUL << 16) | (7UL << 8));
    ARMM_SCB_AIRCR = ARMM_AIRCR_VECTKEY | v | ((group & 7u) << 8);
    armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_pendsv_set(void) { ARMM_SCB_ICSR = ARMM_ICSR_PENDSVSET; }
ARMM_ALWAYS_INLINE void armm_pendsv_clear(void) { ARMM_SCB_ICSR = ARMM_ICSR_PENDSVCLR; }
ARMM_ALWAYS_INLINE void armm_systick_pending_set(void) { ARMM_SCB_ICSR = ARMM_ICSR_PENDSTSET; }
ARMM_ALWAYS_INLINE void armm_systick_pending_clear(void) { ARMM_SCB_ICSR = ARMM_ICSR_PENDSTCLR; }
ARMM_ALWAYS_INLINE void armm_sleepdeep_enable(void) { ARMM_SCB_SCR |= ARMM_SCR_SLEEPDEEP; }
ARMM_ALWAYS_INLINE void armm_sleepdeep_disable(void) { ARMM_SCB_SCR &= ~ARMM_SCR_SLEEPDEEP; }
ARMM_ALWAYS_INLINE void armm_sleep_on_exit_enable(void) { ARMM_SCB_SCR |= ARMM_SCR_SLEEPONEXIT; }
ARMM_ALWAYS_INLINE void armm_sleep_on_exit_disable(void) { ARMM_SCB_SCR &= ~ARMM_SCR_SLEEPONEXIT; }
#if ARMM_HAS_VTOR
ARMM_ALWAYS_INLINE uintptr_t armm_vector_table_get(void) { return (uintptr_t)ARMM_SCB_VTOR; }
ARMM_ALWAYS_INLINE void armm_vector_table_set(uintptr_t addr) { ARMM_SCB_VTOR = (uint32_t)addr; armm_dsb(); armm_isb(); }
#endif
ARMM_ALWAYS_INLINE void armm_system_handler_priority_set(uint32_t exception_number, uint32_t priority)
{
    if (exception_number >= 4u && exception_number <= 15u)
        ARMM_SCB_SHPR(exception_number - 4u) = (uint8_t)(priority << (8u - ARMM_CFG_NVIC_PRIO_BITS));
}
ARMM_ALWAYS_INLINE ARMM_NORETURN void armm_system_reset(void)
{
    armm_dsb();
    ARMM_SCB_AIRCR = ARMM_AIRCR_VECTKEY | ARMM_AIRCR_SYSRESETREQ;
    armm_dsb();
    for (;;) armm_nop();
}

#if ARMM_HAS_CFSR
typedef struct {
    uint32_t cfsr, hfsr, dfsr, mmfar, bfar, afsr, shcsr;
} armm_fault_snapshot_t;
ARMM_ALWAYS_INLINE armm_fault_snapshot_t armm_fault_snapshot(void)
{
    armm_fault_snapshot_t f = { ARMM_SCB_CFSR, ARMM_SCB_HFSR, ARMM_SCB_DFSR,
                                ARMM_SCB_MMFAR, ARMM_SCB_BFAR, ARMM_SCB_AFSR,
                                ARMM_SCB_SHCSR };
    return f;
}
#endif

#endif
