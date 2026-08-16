#ifndef CM4U_H
#define CM4U_H

/*
 * cm4u.h - tiny standalone Cortex-M4 core utility layer
 *
 * Header-only, register-level, no CMSIS dependency.
 * Target: Armv7E-M / Cortex-M4.
 *
 * Scope: core registers, NVIC, SCB, SysTick, DWT, CoreDebug, MPU, FPU,
 *        exception state, barriers, low-power, exclusive accesses, profiling,
 *        delays, fault inspection, and common bit/instruction helpers.
 *
 * This is intentionally NOT a vendor peripheral HAL/BSP. Vendor clocks,
 * GPIO, UART, DMA, ADC, etc. belong in a device/platform layer above this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Configuration
 * -------------------------------------------------------------------------- */

#ifndef CM4U_NVIC_PRIO_BITS
#define CM4U_NVIC_PRIO_BITS 4u
#endif

#ifndef CM4U_HAS_MPU
#define CM4U_HAS_MPU 1u
#endif

#ifndef CM4U_HAS_FPU
#define CM4U_HAS_FPU 1u
#endif

#if !defined(__arm__) && !defined(__thumb__)
#define CM4U_HOST_BUILD 1
#else
#define CM4U_HOST_BUILD 0
#endif

#if defined(__GNUC__) || defined(__clang__)
#define CM4U_ALWAYS_INLINE static inline __attribute__((always_inline))
#define CM4U_NORETURN __attribute__((noreturn))
#define CM4U_PACKED __attribute__((packed))
#else
#define CM4U_ALWAYS_INLINE static inline
#define CM4U_NORETURN
#define CM4U_PACKED
#endif

#define CM4U_BIT(n) (1UL << (n))
#define CM4U_FIELD_MASK(width, pos) (((1UL << (width)) - 1UL) << (pos))

/* --------------------------------------------------------------------------
 * Cortex-M4 System Control Space addresses
 * -------------------------------------------------------------------------- */

#define CM4U_ITM_BASE       0xE0000000UL
#define CM4U_DWT_BASE       0xE0001000UL
#define CM4U_SCS_BASE       0xE000E000UL
#define CM4U_SYSTICK_BASE   0xE000E010UL
#define CM4U_NVIC_BASE      0xE000E100UL
#define CM4U_SCB_BASE       0xE000ED00UL
#define CM4U_MPU_BASE       0xE000ED90UL
#define CM4U_COREDEBUG_BASE 0xE000EDF0UL
#define CM4U_FPU_BASE       0xE000EF30UL
#define CM4U_ICTR_ADDR       0xE000E004UL
#define CM4U_ACTLR_ADDR      0xE000E008UL

/* --------------------------------------------------------------------------
 * Register layouts
 * -------------------------------------------------------------------------- */

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile const uint32_t CALIB;
} cm4u_systick_regs_t;

typedef struct {
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    volatile const uint32_t IABR[8];
    uint32_t RESERVED4[56];
    volatile uint8_t IP[240];
    uint32_t RESERVED5[644];
    volatile uint32_t STIR;
} cm4u_nvic_regs_t;

typedef struct {
    volatile const uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint8_t SHP[12];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t DFSR;
    volatile uint32_t MMFAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
    volatile const uint32_t PFR[2];
    volatile const uint32_t DFR;
    volatile const uint32_t ADR;
    volatile const uint32_t MMFR[4];
    volatile const uint32_t ISAR[5];
    uint32_t RESERVED0[5];
    volatile uint32_t CPACR;
} cm4u_scb_regs_t;

typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
    volatile uint32_t CPICNT;
    volatile uint32_t EXCCNT;
    volatile uint32_t SLEEPCNT;
    volatile uint32_t LSUCNT;
    volatile uint32_t FOLDCNT;
    volatile const uint32_t PCSR;
    volatile uint32_t COMP0;
    volatile uint32_t MASK0;
    volatile uint32_t FUNCTION0;
    uint32_t RESERVED0;
    volatile uint32_t COMP1;
    volatile uint32_t MASK1;
    volatile uint32_t FUNCTION1;
    uint32_t RESERVED1;
    volatile uint32_t COMP2;
    volatile uint32_t MASK2;
    volatile uint32_t FUNCTION2;
    uint32_t RESERVED2;
    volatile uint32_t COMP3;
    volatile uint32_t MASK3;
    volatile uint32_t FUNCTION3;
} cm4u_dwt_regs_t;

typedef struct {
    volatile uint32_t DHCSR;
    volatile uint32_t DCRSR;
    volatile uint32_t DCRDR;
    volatile uint32_t DEMCR;
} cm4u_coredebug_regs_t;

typedef struct {
    volatile const uint32_t TYPE;
    volatile uint32_t CTRL;
    volatile uint32_t RNR;
    volatile uint32_t RBAR;
    volatile uint32_t RASR;
} cm4u_mpu_regs_t;

typedef struct {
    uint32_t RESERVED0;
    volatile uint32_t FPCCR;
    volatile uint32_t FPCAR;
    volatile uint32_t FPDSCR;
    volatile const uint32_t MVFR0;
    volatile const uint32_t MVFR1;
} cm4u_fpu_regs_t;

#define CM4U_SYSTICK   ((cm4u_systick_regs_t *)CM4U_SYSTICK_BASE)
#define CM4U_NVIC      ((cm4u_nvic_regs_t *)CM4U_NVIC_BASE)
#define CM4U_SCB       ((cm4u_scb_regs_t *)CM4U_SCB_BASE)
#define CM4U_DWT       ((cm4u_dwt_regs_t *)CM4U_DWT_BASE)
#define CM4U_COREDEBUG ((cm4u_coredebug_regs_t *)CM4U_COREDEBUG_BASE)
#define CM4U_MPU       ((cm4u_mpu_regs_t *)CM4U_MPU_BASE)
#define CM4U_FPU       ((cm4u_fpu_regs_t *)CM4U_FPU_BASE)

/* --------------------------------------------------------------------------
 * Bit definitions used by the API
 * -------------------------------------------------------------------------- */

#define CM4U_SYSTICK_CTRL_ENABLE      CM4U_BIT(0)
#define CM4U_SYSTICK_CTRL_TICKINT     CM4U_BIT(1)
#define CM4U_SYSTICK_CTRL_CLKSOURCE   CM4U_BIT(2)
#define CM4U_SYSTICK_CTRL_COUNTFLAG   CM4U_BIT(16)
#define CM4U_SYSTICK_RELOAD_MAX       0x00FFFFFFUL

#define CM4U_SCB_ICSR_VECTACTIVE_MASK 0x1FFUL
#define CM4U_SCB_ICSR_RETTOBASE       CM4U_BIT(11)
#define CM4U_SCB_ICSR_VECTPENDING_MASK (0x1FFUL << 12)
#define CM4U_SCB_ICSR_ISRPENDING      CM4U_BIT(22)
#define CM4U_SCB_ICSR_PENDSTCLR       CM4U_BIT(25)
#define CM4U_SCB_ICSR_PENDSTSET       CM4U_BIT(26)
#define CM4U_SCB_ICSR_PENDSVCLR       CM4U_BIT(27)
#define CM4U_SCB_ICSR_PENDSVSET       CM4U_BIT(28)
#define CM4U_SCB_ICSR_NMIPENDSET      CM4U_BIT(31)

#define CM4U_SCB_AIRCR_VECTKEY        (0x5FAUL << 16)
#define CM4U_SCB_AIRCR_PRIGROUP_MASK  (7UL << 8)
#define CM4U_SCB_AIRCR_SYSRESETREQ    CM4U_BIT(2)
#define CM4U_SCB_AIRCR_VECTCLRACTIVE  CM4U_BIT(1)

#define CM4U_SCB_SCR_SLEEPONEXIT      CM4U_BIT(1)
#define CM4U_SCB_SCR_SLEEPDEEP        CM4U_BIT(2)
#define CM4U_SCB_SCR_SEVONPEND        CM4U_BIT(4)

#define CM4U_SCB_CCR_NONBASETHRDENA   CM4U_BIT(0)
#define CM4U_SCB_CCR_USERSETMPEND     CM4U_BIT(1)
#define CM4U_SCB_CCR_UNALIGN_TRP      CM4U_BIT(3)
#define CM4U_SCB_CCR_DIV_0_TRP        CM4U_BIT(4)
#define CM4U_SCB_CCR_BFHFNMIGN        CM4U_BIT(8)
#define CM4U_SCB_CCR_STKALIGN         CM4U_BIT(9)

#define CM4U_SCB_SHCSR_MEMFAULTENA    CM4U_BIT(16)
#define CM4U_SCB_SHCSR_BUSFAULTENA    CM4U_BIT(17)
#define CM4U_SCB_SHCSR_USGFAULTENA    CM4U_BIT(18)

#define CM4U_COREDEBUG_DEMCR_TRCENA   CM4U_BIT(24)
#define CM4U_COREDEBUG_DEMCR_MON_EN   CM4U_BIT(16)
#define CM4U_COREDEBUG_DEMCR_MON_PEND CM4U_BIT(17)
#define CM4U_COREDEBUG_DEMCR_MON_STEP CM4U_BIT(18)
#define CM4U_COREDEBUG_DEMCR_MON_REQ  CM4U_BIT(19)

#define CM4U_DWT_CTRL_CYCCNTENA       CM4U_BIT(0)
#define CM4U_DWT_CTRL_CPIEVTENA       CM4U_BIT(17)
#define CM4U_DWT_CTRL_EXCEVTENA       CM4U_BIT(18)
#define CM4U_DWT_CTRL_SLEEPEVTENA     CM4U_BIT(19)
#define CM4U_DWT_CTRL_LSUEVTENA       CM4U_BIT(20)
#define CM4U_DWT_CTRL_FOLDEVTENA      CM4U_BIT(21)
#define CM4U_DWT_CTRL_CYCEVTENA       CM4U_BIT(22)
#define CM4U_DWT_CTRL_NOPRFCNT        CM4U_BIT(24)
#define CM4U_DWT_CTRL_NOCYCCNT        CM4U_BIT(25)
#define CM4U_DWT_CTRL_NUMCOMP_MASK    (0xFUL << 28)

#define CM4U_MPU_CTRL_ENABLE          CM4U_BIT(0)
#define CM4U_MPU_CTRL_HFNMIENA        CM4U_BIT(1)
#define CM4U_MPU_CTRL_PRIVDEFENA      CM4U_BIT(2)
#define CM4U_MPU_RASR_ENABLE          CM4U_BIT(0)

#define CM4U_FPU_CPACR_CP10_CP11_FULL (0xFUL << 20)
#define CM4U_FPU_FPCCR_LSPEN          CM4U_BIT(30)
#define CM4U_FPU_FPCCR_ASPEN          CM4U_BIT(31)

/* --------------------------------------------------------------------------
 * Exception numbers (architectural exceptions use negative IRQn convention)
 * -------------------------------------------------------------------------- */

typedef int32_t cm4u_irqn_t;

enum {
    CM4U_IRQ_NMI       = -14,
    CM4U_IRQ_HARDFAULT = -13,
    CM4U_IRQ_MEMMANAGE = -12,
    CM4U_IRQ_BUSFAULT  = -11,
    CM4U_IRQ_USAGEFAULT= -10,
    CM4U_IRQ_SVCALL    = -5,
    CM4U_IRQ_DEBUGMON  = -4,
    CM4U_IRQ_PENDSV    = -2,
    CM4U_IRQ_SYSTICK   = -1
};

typedef enum {
    CM4U_MODE_THREAD = 0,
    CM4U_MODE_HANDLER = 1
} cm4u_mode_t;

typedef enum {
    CM4U_STACK_MSP = 0,
    CM4U_STACK_PSP = 1
} cm4u_stack_t;

/* --------------------------------------------------------------------------
 * Compiler/core instruction intrinsics
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE void cm4u_nop(void) { __asm volatile ("nop"); }
CM4U_ALWAYS_INLINE void cm4u_wfi(void) { __asm volatile ("wfi" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_wfe(void) { __asm volatile ("wfe" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_sev(void) { __asm volatile ("sev" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_isb(void) { __asm volatile ("isb 0xF" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_dsb(void) { __asm volatile ("dsb 0xF" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_dmb(void) { __asm volatile ("dmb 0xF" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_clrex(void) { __asm volatile ("clrex" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_svc0(void) { __asm volatile ("svc 0" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_bkpt0(void) { __asm volatile ("bkpt 0"); }

CM4U_ALWAYS_INLINE uint32_t cm4u_get_apsr(void) { uint32_t v; __asm volatile ("mrs %0, apsr" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_ipsr(void) { uint32_t v; __asm volatile ("mrs %0, ipsr" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_xpsr(void) { uint32_t v; __asm volatile ("mrs %0, xpsr" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_control(void) { uint32_t v; __asm volatile ("mrs %0, control" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_primask(void) { uint32_t v; __asm volatile ("mrs %0, primask" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_basepri(void) { uint32_t v; __asm volatile ("mrs %0, basepri" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_faultmask(void) { uint32_t v; __asm volatile ("mrs %0, faultmask" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_msp(void) { uint32_t v; __asm volatile ("mrs %0, msp" : "=r"(v)); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_get_psp(void) { uint32_t v; __asm volatile ("mrs %0, psp" : "=r"(v)); return v; }

CM4U_ALWAYS_INLINE void cm4u_set_control(uint32_t v) { __asm volatile ("msr control, %0" :: "r"(v) : "memory"); cm4u_isb(); }
CM4U_ALWAYS_INLINE void cm4u_set_primask(uint32_t v) { __asm volatile ("msr primask, %0" :: "r"(v) : "memory"); }
CM4U_ALWAYS_INLINE void cm4u_set_basepri(uint32_t v) { __asm volatile ("msr basepri, %0" :: "r"(v) : "memory"); }
CM4U_ALWAYS_INLINE void cm4u_set_basepri_max(uint32_t v) { __asm volatile ("msr basepri_max, %0" :: "r"(v) : "memory"); }
CM4U_ALWAYS_INLINE void cm4u_set_faultmask(uint32_t v) { __asm volatile ("msr faultmask, %0" :: "r"(v) : "memory"); }
CM4U_ALWAYS_INLINE void cm4u_set_msp(uint32_t v) { __asm volatile ("msr msp, %0" :: "r"(v) : "memory"); }
CM4U_ALWAYS_INLINE void cm4u_set_psp(uint32_t v) { __asm volatile ("msr psp, %0" :: "r"(v) : "memory"); }

CM4U_ALWAYS_INLINE void cm4u_irq_disable(void) { __asm volatile ("cpsid i" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_irq_enable(void) { __asm volatile ("cpsie i" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_fault_irq_disable(void) { __asm volatile ("cpsid f" ::: "memory"); }
CM4U_ALWAYS_INLINE void cm4u_fault_irq_enable(void) { __asm volatile ("cpsie f" ::: "memory"); }

CM4U_ALWAYS_INLINE uint32_t cm4u_ldrex32(volatile uint32_t *addr) { uint32_t v; __asm volatile ("ldrex %0, [%1]" : "=&r"(v) : "r"(addr) : "memory"); return v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_strex32(uint32_t v, volatile uint32_t *addr) { uint32_t r; __asm volatile ("strex %0, %2, [%1]" : "=&r"(r) : "r"(addr), "r"(v) : "memory"); return r; }
CM4U_ALWAYS_INLINE uint16_t cm4u_ldrex16(volatile uint16_t *addr) { uint32_t v; __asm volatile ("ldrexh %0, [%1]" : "=&r"(v) : "r"(addr) : "memory"); return (uint16_t)v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_strex16(uint16_t v, volatile uint16_t *addr) { uint32_t r; __asm volatile ("strexh %0, %2, [%1]" : "=&r"(r) : "r"(addr), "r"((uint32_t)v) : "memory"); return r; }
CM4U_ALWAYS_INLINE uint8_t cm4u_ldrex8(volatile uint8_t *addr) { uint32_t v; __asm volatile ("ldrexb %0, [%1]" : "=&r"(v) : "r"(addr) : "memory"); return (uint8_t)v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_strex8(uint8_t v, volatile uint8_t *addr) { uint32_t r; __asm volatile ("strexb %0, %2, [%1]" : "=&r"(r) : "r"(addr), "r"((uint32_t)v) : "memory"); return r; }

/* --------------------------------------------------------------------------
 * Common integer/bit intrinsics
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE uint32_t cm4u_rev32(uint32_t v) { return __builtin_bswap32(v); }
CM4U_ALWAYS_INLINE uint16_t cm4u_rev16(uint16_t v) { return __builtin_bswap16(v); }
CM4U_ALWAYS_INLINE uint32_t cm4u_rbit32(uint32_t v) {
#if defined(__clang__) && __has_builtin(__builtin_arm_rbit)
    return __builtin_arm_rbit(v);
#else
    uint32_t r; __asm volatile ("rbit %0, %1" : "=r"(r) : "r"(v)); return r;
#endif
}
CM4U_ALWAYS_INLINE uint32_t cm4u_clz32(uint32_t v) { return v ? (uint32_t)__builtin_clz(v) : 32u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_popcount32(uint32_t v) { return (uint32_t)__builtin_popcount(v); }
CM4U_ALWAYS_INLINE int32_t cm4u_ssat(int32_t v, uint32_t bits) {
    if (bits == 0u) return 0;
    if (bits >= 32u) return v;
    int64_t hi = ((int64_t)1 << (bits - 1u)) - 1;
    int64_t lo = -((int64_t)1 << (bits - 1u));
    if ((int64_t)v > hi) return (int32_t)hi;
    if ((int64_t)v < lo) return (int32_t)lo;
    return v;
}
CM4U_ALWAYS_INLINE uint32_t cm4u_usat(int32_t v, uint32_t bits) {
    if (v <= 0) return 0u;
    if (bits >= 32u) return (uint32_t)v;
    uint32_t hi = (1u << bits) - 1u;
    return ((uint32_t)v > hi) ? hi : (uint32_t)v;
}
CM4U_ALWAYS_INLINE uint32_t cm4u_ror32(uint32_t v, uint32_t n) { n &= 31u; return n ? ((v >> n) | (v << (32u - n))) : v; }

/* --------------------------------------------------------------------------
 * Execution mode, privilege, and stack helpers
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE cm4u_mode_t cm4u_mode(void) { return (cm4u_get_ipsr() == 0u) ? CM4U_MODE_THREAD : CM4U_MODE_HANDLER; }
CM4U_ALWAYS_INLINE bool cm4u_in_thread_mode(void) { return cm4u_get_ipsr() == 0u; }
CM4U_ALWAYS_INLINE bool cm4u_in_handler_mode(void) { return cm4u_get_ipsr() != 0u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_exception_number(void) { return cm4u_get_ipsr() & 0x1FFu; }
CM4U_ALWAYS_INLINE bool cm4u_thread_is_privileged(void) { return (cm4u_get_control() & 1u) == 0u; }
CM4U_ALWAYS_INLINE bool cm4u_thread_is_unprivileged(void) { return (cm4u_get_control() & 1u) != 0u; }
CM4U_ALWAYS_INLINE cm4u_stack_t cm4u_thread_stack(void) { return (cm4u_get_control() & 2u) ? CM4U_STACK_PSP : CM4U_STACK_MSP; }
CM4U_ALWAYS_INLINE bool cm4u_using_msp(void) { return cm4u_in_handler_mode() || ((cm4u_get_control() & 2u) == 0u); }
CM4U_ALWAYS_INLINE bool cm4u_using_psp(void) { return cm4u_in_thread_mode() && ((cm4u_get_control() & 2u) != 0u); }
CM4U_ALWAYS_INLINE void cm4u_thread_use_msp(void) { cm4u_set_control(cm4u_get_control() & ~2u); }
CM4U_ALWAYS_INLINE void cm4u_thread_use_psp(void) { cm4u_set_control(cm4u_get_control() | 2u); }
CM4U_ALWAYS_INLINE void cm4u_thread_drop_privilege(void) { cm4u_set_control(cm4u_get_control() | 1u); }

/* --------------------------------------------------------------------------
 * Critical sections
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE uint32_t cm4u_critical_enter(void) { uint32_t s = cm4u_get_primask(); cm4u_irq_disable(); return s; }
CM4U_ALWAYS_INLINE void cm4u_critical_exit(uint32_t saved_primask) { cm4u_set_primask(saved_primask); }
CM4U_ALWAYS_INLINE uint32_t cm4u_basepri_enter(uint32_t priority_mask) { uint32_t s = cm4u_get_basepri(); cm4u_set_basepri_max(priority_mask); return s; }
CM4U_ALWAYS_INLINE void cm4u_basepri_exit(uint32_t saved_basepri) { cm4u_set_basepri(saved_basepri); }

/* --------------------------------------------------------------------------
 * NVIC helpers
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE bool cm4u_nvic_valid_external_irq(cm4u_irqn_t irqn) { return (irqn >= 0) && ((uint32_t)irqn < 240u); }
CM4U_ALWAYS_INLINE void cm4u_nvic_enable_irq(cm4u_irqn_t irqn) { if (cm4u_nvic_valid_external_irq(irqn)) CM4U_NVIC->ISER[(uint32_t)irqn >> 5] = 1u << ((uint32_t)irqn & 31u); }
CM4U_ALWAYS_INLINE void cm4u_nvic_disable_irq(cm4u_irqn_t irqn) { if (cm4u_nvic_valid_external_irq(irqn)) { CM4U_NVIC->ICER[(uint32_t)irqn >> 5] = 1u << ((uint32_t)irqn & 31u); cm4u_dsb(); cm4u_isb(); } }
CM4U_ALWAYS_INLINE bool cm4u_nvic_irq_enabled(cm4u_irqn_t irqn) { return cm4u_nvic_valid_external_irq(irqn) && ((CM4U_NVIC->ISER[(uint32_t)irqn >> 5] & (1u << ((uint32_t)irqn & 31u))) != 0u); }
CM4U_ALWAYS_INLINE void cm4u_nvic_set_pending(cm4u_irqn_t irqn) { if (cm4u_nvic_valid_external_irq(irqn)) CM4U_NVIC->ISPR[(uint32_t)irqn >> 5] = 1u << ((uint32_t)irqn & 31u); }
CM4U_ALWAYS_INLINE void cm4u_nvic_clear_pending(cm4u_irqn_t irqn) { if (cm4u_nvic_valid_external_irq(irqn)) CM4U_NVIC->ICPR[(uint32_t)irqn >> 5] = 1u << ((uint32_t)irqn & 31u); }
CM4U_ALWAYS_INLINE bool cm4u_nvic_is_pending(cm4u_irqn_t irqn) { return cm4u_nvic_valid_external_irq(irqn) && ((CM4U_NVIC->ISPR[(uint32_t)irqn >> 5] & (1u << ((uint32_t)irqn & 31u))) != 0u); }
CM4U_ALWAYS_INLINE bool cm4u_nvic_is_active(cm4u_irqn_t irqn) { return cm4u_nvic_valid_external_irq(irqn) && ((CM4U_NVIC->IABR[(uint32_t)irqn >> 5] & (1u << ((uint32_t)irqn & 31u))) != 0u); }
CM4U_ALWAYS_INLINE void cm4u_nvic_trigger_irq(cm4u_irqn_t irqn) { if (cm4u_nvic_valid_external_irq(irqn)) CM4U_NVIC->STIR = ((uint32_t)irqn & 0x1FFu); }

CM4U_ALWAYS_INLINE uint8_t cm4u_nvic_priority_encode(uint32_t priority) { return (uint8_t)((priority << (8u - CM4U_NVIC_PRIO_BITS)) & 0xFFu); }
CM4U_ALWAYS_INLINE uint32_t cm4u_nvic_priority_decode(uint8_t encoded) { return (uint32_t)encoded >> (8u - CM4U_NVIC_PRIO_BITS); }

CM4U_ALWAYS_INLINE void cm4u_nvic_set_priority(cm4u_irqn_t irqn, uint32_t priority) {
    uint8_t p = cm4u_nvic_priority_encode(priority);
    if (irqn >= 0) {
        if (cm4u_nvic_valid_external_irq(irqn)) CM4U_NVIC->IP[(uint32_t)irqn] = p;
    } else {
        int32_t exc = irqn + 16;
        if ((exc >= 4) && (exc <= 15) && (exc != 7) && (exc != 8) && (exc != 9) && (exc != 10) && (exc != 13)) CM4U_SCB->SHP[(uint32_t)exc - 4u] = p;
    }
}
CM4U_ALWAYS_INLINE uint32_t cm4u_nvic_get_priority(cm4u_irqn_t irqn) {
    if (irqn >= 0) return cm4u_nvic_valid_external_irq(irqn) ? cm4u_nvic_priority_decode(CM4U_NVIC->IP[(uint32_t)irqn]) : 0u;
    int32_t exc = irqn + 16;
    return ((exc >= 4) && (exc <= 15)) ? cm4u_nvic_priority_decode(CM4U_SCB->SHP[(uint32_t)exc - 4u]) : 0u;
}

CM4U_ALWAYS_INLINE void cm4u_nvic_set_priority_grouping(uint32_t group) {
    uint32_t reg = CM4U_SCB->AIRCR;
    reg &= ~(0xFFFFUL << 16);
    reg &= ~CM4U_SCB_AIRCR_PRIGROUP_MASK;
    reg |= CM4U_SCB_AIRCR_VECTKEY | ((group & 7u) << 8);
    CM4U_SCB->AIRCR = reg;
}
CM4U_ALWAYS_INLINE uint32_t cm4u_nvic_get_priority_grouping(void) { return (CM4U_SCB->AIRCR >> 8) & 7u; }

CM4U_ALWAYS_INLINE uint32_t cm4u_nvic_encode_priority(uint32_t group, uint32_t preempt_prio, uint32_t sub_prio) {
    uint32_t pg = group & 7u;
    uint32_t pre_bits = ((7u - pg) > CM4U_NVIC_PRIO_BITS) ? CM4U_NVIC_PRIO_BITS : (7u - pg);
    uint32_t sub_bits = ((pg + CM4U_NVIC_PRIO_BITS) < 7u) ? 0u : (pg - 7u + CM4U_NVIC_PRIO_BITS);
    uint32_t pre_mask = (pre_bits == 0u) ? 0u : ((1u << pre_bits) - 1u);
    uint32_t sub_mask = (sub_bits == 0u) ? 0u : ((1u << sub_bits) - 1u);
    return ((preempt_prio & pre_mask) << sub_bits) | (sub_prio & sub_mask);
}
CM4U_ALWAYS_INLINE void cm4u_nvic_decode_priority(uint32_t priority, uint32_t group, uint32_t *preempt_prio, uint32_t *sub_prio) {
    uint32_t pg = group & 7u;
    uint32_t pre_bits = ((7u - pg) > CM4U_NVIC_PRIO_BITS) ? CM4U_NVIC_PRIO_BITS : (7u - pg);
    uint32_t sub_bits = ((pg + CM4U_NVIC_PRIO_BITS) < 7u) ? 0u : (pg - 7u + CM4U_NVIC_PRIO_BITS);
    uint32_t pre_mask = (pre_bits == 0u) ? 0u : ((1u << pre_bits) - 1u);
    uint32_t sub_mask = (sub_bits == 0u) ? 0u : ((1u << sub_bits) - 1u);
    if (preempt_prio) *preempt_prio = (priority >> sub_bits) & pre_mask;
    if (sub_prio) *sub_prio = priority & sub_mask;
}

/* --------------------------------------------------------------------------
 * SCB, exceptions, faults, reset, vector table
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE uint32_t cm4u_scb_cpuid(void) { return CM4U_SCB->CPUID; }
CM4U_ALWAYS_INLINE uint32_t cm4u_scb_icsr(void) { return CM4U_SCB->ICSR; }
CM4U_ALWAYS_INLINE uint32_t cm4u_active_exception(void) { return CM4U_SCB->ICSR & CM4U_SCB_ICSR_VECTACTIVE_MASK; }
CM4U_ALWAYS_INLINE uint32_t cm4u_pending_exception(void) { return (CM4U_SCB->ICSR >> 12) & 0x1FFu; }
CM4U_ALWAYS_INLINE bool cm4u_exception_pending_any(void) { return (CM4U_SCB->ICSR & CM4U_SCB_ICSR_ISRPENDING) != 0u; }
CM4U_ALWAYS_INLINE bool cm4u_exception_return_to_base(void) { return (CM4U_SCB->ICSR & CM4U_SCB_ICSR_RETTOBASE) != 0u; }
CM4U_ALWAYS_INLINE void cm4u_pendsv_set(void) { CM4U_SCB->ICSR = CM4U_SCB_ICSR_PENDSVSET; }
CM4U_ALWAYS_INLINE void cm4u_pendsv_clear(void) { CM4U_SCB->ICSR = CM4U_SCB_ICSR_PENDSVCLR; }
CM4U_ALWAYS_INLINE void cm4u_systick_pending_set(void) { CM4U_SCB->ICSR = CM4U_SCB_ICSR_PENDSTSET; }
CM4U_ALWAYS_INLINE void cm4u_systick_pending_clear(void) { CM4U_SCB->ICSR = CM4U_SCB_ICSR_PENDSTCLR; }
CM4U_ALWAYS_INLINE void cm4u_nmi_trigger(void) { CM4U_SCB->ICSR = CM4U_SCB_ICSR_NMIPENDSET; }
CM4U_ALWAYS_INLINE uintptr_t cm4u_vector_table_get(void) { return (uintptr_t)CM4U_SCB->VTOR; }
CM4U_ALWAYS_INLINE void cm4u_vector_table_set(uintptr_t address) { CM4U_SCB->VTOR = (uint32_t)address; cm4u_dsb(); cm4u_isb(); }
CM4U_ALWAYS_INLINE uint32_t cm4u_interrupt_line_groups(void) { return ((*(volatile const uint32_t *)CM4U_ICTR_ADDR) & 0xFu) + 1u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_actlr_get(void) { return *(volatile const uint32_t *)CM4U_ACTLR_ADDR; }
CM4U_ALWAYS_INLINE uintptr_t cm4u_vector_get(cm4u_irqn_t irqn) {
    int32_t index = irqn + 16;
    if (index < 0) return (uintptr_t)0;
    volatile const uint32_t *table = (volatile const uint32_t *)cm4u_vector_table_get();
    return (uintptr_t)table[(uint32_t)index];
}
CM4U_ALWAYS_INLINE bool cm4u_vector_set(cm4u_irqn_t irqn, uintptr_t handler) {
    int32_t index = irqn + 16;
    if (index < 0) return false;
    volatile uint32_t *table = (volatile uint32_t *)cm4u_vector_table_get();
    table[(uint32_t)index] = (uint32_t)handler;
    cm4u_dsb(); cm4u_isb();
    return true;
}

CM4U_ALWAYS_INLINE void cm4u_sleep_on_exit_enable(void) { CM4U_SCB->SCR |= CM4U_SCB_SCR_SLEEPONEXIT; }
CM4U_ALWAYS_INLINE void cm4u_sleep_on_exit_disable(void) { CM4U_SCB->SCR &= ~CM4U_SCB_SCR_SLEEPONEXIT; }
CM4U_ALWAYS_INLINE void cm4u_sleepdeep_enable(void) { CM4U_SCB->SCR |= CM4U_SCB_SCR_SLEEPDEEP; }
CM4U_ALWAYS_INLINE void cm4u_sleepdeep_disable(void) { CM4U_SCB->SCR &= ~CM4U_SCB_SCR_SLEEPDEEP; }
CM4U_ALWAYS_INLINE void cm4u_sev_on_pend_enable(void) { CM4U_SCB->SCR |= CM4U_SCB_SCR_SEVONPEND; }
CM4U_ALWAYS_INLINE void cm4u_sev_on_pend_disable(void) { CM4U_SCB->SCR &= ~CM4U_SCB_SCR_SEVONPEND; }

CM4U_ALWAYS_INLINE void cm4u_div0_trap_enable(void) { CM4U_SCB->CCR |= CM4U_SCB_CCR_DIV_0_TRP; }
CM4U_ALWAYS_INLINE void cm4u_div0_trap_disable(void) { CM4U_SCB->CCR &= ~CM4U_SCB_CCR_DIV_0_TRP; }
CM4U_ALWAYS_INLINE void cm4u_unalign_trap_enable(void) { CM4U_SCB->CCR |= CM4U_SCB_CCR_UNALIGN_TRP; }
CM4U_ALWAYS_INLINE void cm4u_unalign_trap_disable(void) { CM4U_SCB->CCR &= ~CM4U_SCB_CCR_UNALIGN_TRP; }
CM4U_ALWAYS_INLINE void cm4u_stack_align8_enable(void) { CM4U_SCB->CCR |= CM4U_SCB_CCR_STKALIGN; }
CM4U_ALWAYS_INLINE void cm4u_stack_align8_disable(void) { CM4U_SCB->CCR &= ~CM4U_SCB_CCR_STKALIGN; }

CM4U_ALWAYS_INLINE void cm4u_memfault_enable(void) { CM4U_SCB->SHCSR |= CM4U_SCB_SHCSR_MEMFAULTENA; }
CM4U_ALWAYS_INLINE void cm4u_busfault_enable(void) { CM4U_SCB->SHCSR |= CM4U_SCB_SHCSR_BUSFAULTENA; }
CM4U_ALWAYS_INLINE void cm4u_usagefault_enable(void) { CM4U_SCB->SHCSR |= CM4U_SCB_SHCSR_USGFAULTENA; }
CM4U_ALWAYS_INLINE void cm4u_configurable_faults_enable(void) { CM4U_SCB->SHCSR |= CM4U_SCB_SHCSR_MEMFAULTENA | CM4U_SCB_SHCSR_BUSFAULTENA | CM4U_SCB_SHCSR_USGFAULTENA; }
CM4U_ALWAYS_INLINE void cm4u_configurable_faults_disable(void) { CM4U_SCB->SHCSR &= ~(CM4U_SCB_SHCSR_MEMFAULTENA | CM4U_SCB_SHCSR_BUSFAULTENA | CM4U_SCB_SHCSR_USGFAULTENA); }

CM4U_ALWAYS_INLINE uint32_t cm4u_fault_cfsr(void) { return CM4U_SCB->CFSR; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fault_hfsr(void) { return CM4U_SCB->HFSR; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fault_dfsr(void) { return CM4U_SCB->DFSR; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fault_afsr(void) { return CM4U_SCB->AFSR; }
CM4U_ALWAYS_INLINE uintptr_t cm4u_fault_mmfar(void) { return (uintptr_t)CM4U_SCB->MMFAR; }
CM4U_ALWAYS_INLINE uintptr_t cm4u_fault_bfar(void) { return (uintptr_t)CM4U_SCB->BFAR; }
CM4U_ALWAYS_INLINE void cm4u_fault_clear_cfsr(uint32_t mask) { CM4U_SCB->CFSR = mask; }
CM4U_ALWAYS_INLINE void cm4u_fault_clear_hfsr(uint32_t mask) { CM4U_SCB->HFSR = mask; }
CM4U_ALWAYS_INLINE void cm4u_fault_clear_dfsr(uint32_t mask) { CM4U_SCB->DFSR = mask; }

CM4U_ALWAYS_INLINE void cm4u_system_reset(void) {
    cm4u_dsb();
    uint32_t aircr = CM4U_SCB->AIRCR & CM4U_SCB_AIRCR_PRIGROUP_MASK;
    CM4U_SCB->AIRCR = CM4U_SCB_AIRCR_VECTKEY | aircr | CM4U_SCB_AIRCR_SYSRESETREQ;
    cm4u_dsb();
    for (;;) cm4u_nop();
}

/* --------------------------------------------------------------------------
 * SysTick
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE bool cm4u_systick_config(uint32_t ticks) {
    if ((ticks == 0u) || ((ticks - 1u) > CM4U_SYSTICK_RELOAD_MAX)) return false;
    CM4U_SYSTICK->LOAD = ticks - 1u;
    CM4U_SYSTICK->VAL = 0u;
    CM4U_SYSTICK->CTRL = CM4U_SYSTICK_CTRL_CLKSOURCE | CM4U_SYSTICK_CTRL_TICKINT | CM4U_SYSTICK_CTRL_ENABLE;
    return true;
}
CM4U_ALWAYS_INLINE void cm4u_systick_enable(void) { CM4U_SYSTICK->CTRL |= CM4U_SYSTICK_CTRL_ENABLE; }
CM4U_ALWAYS_INLINE void cm4u_systick_disable(void) { CM4U_SYSTICK->CTRL &= ~CM4U_SYSTICK_CTRL_ENABLE; }
CM4U_ALWAYS_INLINE void cm4u_systick_irq_enable(void) { CM4U_SYSTICK->CTRL |= CM4U_SYSTICK_CTRL_TICKINT; }
CM4U_ALWAYS_INLINE void cm4u_systick_irq_disable(void) { CM4U_SYSTICK->CTRL &= ~CM4U_SYSTICK_CTRL_TICKINT; }
CM4U_ALWAYS_INLINE void cm4u_systick_use_core_clock(void) { CM4U_SYSTICK->CTRL |= CM4U_SYSTICK_CTRL_CLKSOURCE; }
CM4U_ALWAYS_INLINE void cm4u_systick_use_external_clock(void) { CM4U_SYSTICK->CTRL &= ~CM4U_SYSTICK_CTRL_CLKSOURCE; }
CM4U_ALWAYS_INLINE void cm4u_systick_reload_set(uint32_t reload) { CM4U_SYSTICK->LOAD = reload & CM4U_SYSTICK_RELOAD_MAX; }
CM4U_ALWAYS_INLINE uint32_t cm4u_systick_reload_get(void) { return CM4U_SYSTICK->LOAD & CM4U_SYSTICK_RELOAD_MAX; }
CM4U_ALWAYS_INLINE uint32_t cm4u_systick_value(void) { return CM4U_SYSTICK->VAL & CM4U_SYSTICK_RELOAD_MAX; }
CM4U_ALWAYS_INLINE void cm4u_systick_value_clear(void) { CM4U_SYSTICK->VAL = 0u; }
CM4U_ALWAYS_INLINE bool cm4u_systick_countflag(void) { return (CM4U_SYSTICK->CTRL & CM4U_SYSTICK_CTRL_COUNTFLAG) != 0u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_systick_calibration(void) { return CM4U_SYSTICK->CALIB; }

/* --------------------------------------------------------------------------
 * DWT: cycle counter, event counters, comparators, profiling, delays
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE bool cm4u_dwt_has_cycle_counter(void) { return (CM4U_DWT->CTRL & CM4U_DWT_CTRL_NOCYCCNT) == 0u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_dwt_comparator_count(void) { return (CM4U_DWT->CTRL >> 28) & 0xFu; }
CM4U_ALWAYS_INLINE bool cm4u_dwt_has_event_counters(void) { return (CM4U_DWT->CTRL & CM4U_DWT_CTRL_NOPRFCNT) == 0u; }
CM4U_ALWAYS_INLINE void cm4u_trace_enable(void) { CM4U_COREDEBUG->DEMCR |= CM4U_COREDEBUG_DEMCR_TRCENA; }
CM4U_ALWAYS_INLINE void cm4u_trace_disable(void) { CM4U_COREDEBUG->DEMCR &= ~CM4U_COREDEBUG_DEMCR_TRCENA; }
CM4U_ALWAYS_INLINE bool cm4u_dwt_init(void) { cm4u_trace_enable(); if (!cm4u_dwt_has_cycle_counter()) return false; CM4U_DWT->CYCCNT = 0u; CM4U_DWT->CTRL |= CM4U_DWT_CTRL_CYCCNTENA; return (CM4U_DWT->CTRL & CM4U_DWT_CTRL_CYCCNTENA) != 0u; }
CM4U_ALWAYS_INLINE void cm4u_dwt_cycle_enable(void) { cm4u_trace_enable(); CM4U_DWT->CTRL |= CM4U_DWT_CTRL_CYCCNTENA; }
CM4U_ALWAYS_INLINE void cm4u_dwt_cycle_disable(void) { CM4U_DWT->CTRL &= ~CM4U_DWT_CTRL_CYCCNTENA; }
CM4U_ALWAYS_INLINE void cm4u_dwt_cycle_reset(void) { CM4U_DWT->CYCCNT = 0u; }
CM4U_ALWAYS_INLINE uint32_t cm4u_cycles(void) { return CM4U_DWT->CYCCNT; }
CM4U_ALWAYS_INLINE uint32_t cm4u_cycles_since(uint32_t start) { return (uint32_t)(CM4U_DWT->CYCCNT - start); }
CM4U_ALWAYS_INLINE uint8_t cm4u_dwt_cpi_count(void) { return (uint8_t)CM4U_DWT->CPICNT; }
CM4U_ALWAYS_INLINE uint8_t cm4u_dwt_exception_count(void) { return (uint8_t)CM4U_DWT->EXCCNT; }
CM4U_ALWAYS_INLINE uint8_t cm4u_dwt_sleep_count(void) { return (uint8_t)CM4U_DWT->SLEEPCNT; }
CM4U_ALWAYS_INLINE uint8_t cm4u_dwt_lsu_count(void) { return (uint8_t)CM4U_DWT->LSUCNT; }
CM4U_ALWAYS_INLINE uint8_t cm4u_dwt_fold_count(void) { return (uint8_t)CM4U_DWT->FOLDCNT; }
CM4U_ALWAYS_INLINE uint32_t cm4u_dwt_pc_sample(void) { return CM4U_DWT->PCSR; }
CM4U_ALWAYS_INLINE void cm4u_dwt_event_counters_reset(void) { CM4U_DWT->CPICNT = 0u; CM4U_DWT->EXCCNT = 0u; CM4U_DWT->SLEEPCNT = 0u; CM4U_DWT->LSUCNT = 0u; CM4U_DWT->FOLDCNT = 0u; }
CM4U_ALWAYS_INLINE void cm4u_dwt_event_counters_enable(void) { CM4U_DWT->CTRL |= CM4U_DWT_CTRL_CPIEVTENA | CM4U_DWT_CTRL_EXCEVTENA | CM4U_DWT_CTRL_SLEEPEVTENA | CM4U_DWT_CTRL_LSUEVTENA | CM4U_DWT_CTRL_FOLDEVTENA; }
CM4U_ALWAYS_INLINE void cm4u_dwt_event_counters_disable(void) { CM4U_DWT->CTRL &= ~(CM4U_DWT_CTRL_CPIEVTENA | CM4U_DWT_CTRL_EXCEVTENA | CM4U_DWT_CTRL_SLEEPEVTENA | CM4U_DWT_CTRL_LSUEVTENA | CM4U_DWT_CTRL_FOLDEVTENA); }

CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_dwt_comp_reg(uint32_t i) { return (volatile uint32_t *)(CM4U_DWT_BASE + 0x20u + i * 0x10u); }
CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_dwt_mask_reg(uint32_t i) { return (volatile uint32_t *)(CM4U_DWT_BASE + 0x24u + i * 0x10u); }
CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_dwt_function_reg(uint32_t i) { return (volatile uint32_t *)(CM4U_DWT_BASE + 0x28u + i * 0x10u); }
CM4U_ALWAYS_INLINE bool cm4u_dwt_comparator_config(uint32_t index, uintptr_t address, uint32_t mask_bits, uint32_t function) {
    if (index >= cm4u_dwt_comparator_count()) return false;
    *cm4u_dwt_comp_reg(index) = (uint32_t)address;
    *cm4u_dwt_mask_reg(index) = mask_bits & 0x1Fu;
    *cm4u_dwt_function_reg(index) = function;
    return true;
}
CM4U_ALWAYS_INLINE void cm4u_dwt_comparator_disable(uint32_t index) { if (index < cm4u_dwt_comparator_count()) *cm4u_dwt_function_reg(index) = 0u; }

CM4U_ALWAYS_INLINE uint32_t cm4u_us_to_cycles(uint32_t us, uint32_t core_hz) { return (uint32_t)(((uint64_t)core_hz * us) / 1000000ULL); }
CM4U_ALWAYS_INLINE uint32_t cm4u_ms_to_cycles(uint32_t ms, uint32_t core_hz) { return (uint32_t)(((uint64_t)core_hz * ms) / 1000ULL); }
CM4U_ALWAYS_INLINE void cm4u_delay_cycles(uint32_t cycles) { uint32_t s = cm4u_cycles(); while (cm4u_cycles_since(s) < cycles) cm4u_nop(); }
CM4U_ALWAYS_INLINE void cm4u_delay_us(uint32_t us, uint32_t core_hz) { while (us != 0u) { uint32_t chunk = (us > 1000000u) ? 1000000u : us; cm4u_delay_cycles(cm4u_us_to_cycles(chunk, core_hz)); us -= chunk; } }
CM4U_ALWAYS_INLINE void cm4u_delay_ms(uint32_t ms, uint32_t core_hz) { while (ms != 0u) { uint32_t chunk = (ms > 1000u) ? 1000u : ms; cm4u_delay_cycles(cm4u_ms_to_cycles(chunk, core_hz)); ms -= chunk; } }
CM4U_ALWAYS_INLINE uint32_t cm4u_profile_begin(void) { return cm4u_cycles(); }
CM4U_ALWAYS_INLINE uint32_t cm4u_profile_end(uint32_t start) { return cm4u_cycles_since(start); }

/* --------------------------------------------------------------------------
 * CoreDebug / DebugMonitor
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE bool cm4u_debugger_attached(void) { return (CM4U_COREDEBUG->DHCSR & 1u) != 0u; }
CM4U_ALWAYS_INLINE void cm4u_debugmon_enable(void) { CM4U_COREDEBUG->DEMCR |= CM4U_COREDEBUG_DEMCR_MON_EN; }
CM4U_ALWAYS_INLINE void cm4u_debugmon_disable(void) { CM4U_COREDEBUG->DEMCR &= ~CM4U_COREDEBUG_DEMCR_MON_EN; }
CM4U_ALWAYS_INLINE void cm4u_debugmon_pend(void) { CM4U_COREDEBUG->DEMCR |= CM4U_COREDEBUG_DEMCR_MON_PEND; }

/* --------------------------------------------------------------------------
 * MPU (Armv7-M PMSAv7)
 * -------------------------------------------------------------------------- */

#define CM4U_MPU_AP_NO_ACCESS          0u
#define CM4U_MPU_AP_PRIV_RW            1u
#define CM4U_MPU_AP_PRIV_RW_USER_RO    2u
#define CM4U_MPU_AP_FULL_ACCESS        3u
#define CM4U_MPU_AP_PRIV_RO            5u
#define CM4U_MPU_AP_RO                 6u

CM4U_ALWAYS_INLINE uint32_t cm4u_mpu_region_count(void) { return (CM4U_MPU->TYPE >> 8) & 0xFFu; }
CM4U_ALWAYS_INLINE bool cm4u_mpu_enabled(void) { return (CM4U_MPU->CTRL & CM4U_MPU_CTRL_ENABLE) != 0u; }
CM4U_ALWAYS_INLINE void cm4u_mpu_disable(void) { cm4u_dmb(); CM4U_MPU->CTRL = 0u; cm4u_dsb(); cm4u_isb(); }
CM4U_ALWAYS_INLINE void cm4u_mpu_enable(bool privileged_default_map, bool enable_during_hardfault_nmi) {
    uint32_t ctrl = CM4U_MPU_CTRL_ENABLE;
    if (privileged_default_map) ctrl |= CM4U_MPU_CTRL_PRIVDEFENA;
    if (enable_during_hardfault_nmi) ctrl |= CM4U_MPU_CTRL_HFNMIENA;
    CM4U_MPU->CTRL = ctrl; cm4u_dsb(); cm4u_isb();
}
CM4U_ALWAYS_INLINE void cm4u_mpu_select_region(uint32_t region) { CM4U_MPU->RNR = region; }
CM4U_ALWAYS_INLINE void cm4u_mpu_region_disable(uint32_t region) { CM4U_MPU->RNR = region; CM4U_MPU->RASR = 0u; cm4u_dsb(); cm4u_isb(); }
CM4U_ALWAYS_INLINE uint32_t cm4u_mpu_size_encode(uint32_t size_bytes) {
    if (size_bytes < 32u || (size_bytes & (size_bytes - 1u)) != 0u) return 0u;
    uint32_t log2 = 31u - cm4u_clz32(size_bytes);
    return (log2 - 1u) & 0x1Fu;
}
CM4U_ALWAYS_INLINE uint32_t cm4u_mpu_rasr_make(bool xn, uint32_t ap, uint32_t tex, bool shareable, bool cacheable, bool bufferable, uint8_t subregion_disable, uint32_t size_bytes) {
    uint32_t size = cm4u_mpu_size_encode(size_bytes);
    return (xn ? CM4U_BIT(28) : 0u) | ((ap & 7u) << 24) | ((tex & 7u) << 19) |
           (shareable ? CM4U_BIT(18) : 0u) | (cacheable ? CM4U_BIT(17) : 0u) | (bufferable ? CM4U_BIT(16) : 0u) |
           ((uint32_t)subregion_disable << 8) | (size << 1) | CM4U_MPU_RASR_ENABLE;
}
CM4U_ALWAYS_INLINE bool cm4u_mpu_region_config(uint32_t region, uintptr_t base, uint32_t rasr) {
    if (region >= cm4u_mpu_region_count()) return false;
    CM4U_MPU->RNR = region;
    CM4U_MPU->RBAR = (uint32_t)base;
    CM4U_MPU->RASR = rasr;
    cm4u_dsb(); cm4u_isb();
    return true;
}

/* --------------------------------------------------------------------------
 * FPU
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE void cm4u_fpu_enable(void) { CM4U_SCB->CPACR |= CM4U_FPU_CPACR_CP10_CP11_FULL; cm4u_dsb(); cm4u_isb(); }
CM4U_ALWAYS_INLINE void cm4u_fpu_disable(void) { CM4U_SCB->CPACR &= ~CM4U_FPU_CPACR_CP10_CP11_FULL; cm4u_dsb(); cm4u_isb(); }
CM4U_ALWAYS_INLINE bool cm4u_fpu_enabled(void) { return (CM4U_SCB->CPACR & CM4U_FPU_CPACR_CP10_CP11_FULL) == CM4U_FPU_CPACR_CP10_CP11_FULL; }
CM4U_ALWAYS_INLINE void cm4u_fpu_lazy_stacking_enable(void) { CM4U_FPU->FPCCR |= CM4U_FPU_FPCCR_ASPEN | CM4U_FPU_FPCCR_LSPEN; }
CM4U_ALWAYS_INLINE void cm4u_fpu_lazy_stacking_disable(void) { CM4U_FPU->FPCCR &= ~CM4U_FPU_FPCCR_LSPEN; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fpu_fpccr(void) { return CM4U_FPU->FPCCR; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fpu_fpdscr(void) { return CM4U_FPU->FPDSCR; }
CM4U_ALWAYS_INLINE void cm4u_fpu_fpdscr_set(uint32_t v) { CM4U_FPU->FPDSCR = v; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fpu_mvfr0(void) { return CM4U_FPU->MVFR0; }
CM4U_ALWAYS_INLINE uint32_t cm4u_fpu_mvfr1(void) { return CM4U_FPU->MVFR1; }

/* --------------------------------------------------------------------------
 * ITM stimulus output (debug trace)
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_itm_stim32(uint32_t port) { return (volatile uint32_t *)(CM4U_ITM_BASE + ((port & 31u) * 4u)); }
CM4U_ALWAYS_INLINE volatile uint8_t *cm4u_itm_stim8(uint32_t port) { return (volatile uint8_t *)(CM4U_ITM_BASE + ((port & 31u) * 4u)); }
CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_itm_ter(void) { return (volatile uint32_t *)(CM4U_ITM_BASE + 0xE00u); }
CM4U_ALWAYS_INLINE volatile uint32_t *cm4u_itm_tcr(void) { return (volatile uint32_t *)(CM4U_ITM_BASE + 0xE80u); }
CM4U_ALWAYS_INLINE bool cm4u_itm_port_enabled(uint32_t port) { return ((*cm4u_itm_tcr() & 1u) != 0u) && ((*cm4u_itm_ter() & (1u << (port & 31u))) != 0u); }
CM4U_ALWAYS_INLINE bool cm4u_itm_try_write8(uint32_t port, uint8_t v) { if (!cm4u_itm_port_enabled(port)) return false; if (*cm4u_itm_stim32(port) == 0u) return false; *cm4u_itm_stim8(port) = v; return true; }

/* --------------------------------------------------------------------------
 * Small atomic helpers using LDREX/STREX
 * -------------------------------------------------------------------------- */

CM4U_ALWAYS_INLINE uint32_t cm4u_atomic_load32(volatile uint32_t *p) { cm4u_dmb(); uint32_t v = *p; cm4u_dmb(); return v; }
CM4U_ALWAYS_INLINE void cm4u_atomic_store32(volatile uint32_t *p, uint32_t v) { cm4u_dmb(); *p = v; cm4u_dmb(); }
CM4U_ALWAYS_INLINE uint32_t cm4u_atomic_fetch_add32(volatile uint32_t *p, uint32_t add) {
    uint32_t old;
    do { old = cm4u_ldrex32(p); } while (cm4u_strex32(old + add, p) != 0u);
    cm4u_dmb();
    return old;
}
CM4U_ALWAYS_INLINE bool cm4u_atomic_compare_exchange32(volatile uint32_t *p, uint32_t *expected, uint32_t desired) {
    uint32_t old;
    for (;;) {
        old = cm4u_ldrex32(p);
        if (old != *expected) { cm4u_clrex(); *expected = old; return false; }
        if (cm4u_strex32(desired, p) == 0u) { cm4u_dmb(); return true; }
    }
}

#ifdef __cplusplus
}
#endif

#endif /* CM4U_H */
