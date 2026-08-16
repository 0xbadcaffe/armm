#ifndef ARMM_INTRINSICS_H
#define ARMM_INTRINSICS_H

#include "armm_arch.h"

#if !defined(__GNUC__) && !defined(__clang__)
#error "armm currently supports GCC and Clang-compatible Arm compilers"
#endif

#define ARMM_ALWAYS_INLINE static inline __attribute__((always_inline))
#define ARMM_NORETURN __attribute__((noreturn))

ARMM_ALWAYS_INLINE void armm_nop(void) { __asm volatile("nop"); }
ARMM_ALWAYS_INLINE void armm_wfi(void) { __asm volatile("wfi" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_wfe(void) { __asm volatile("wfe" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_sev(void) { __asm volatile("sev" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_isb(void) { __asm volatile("isb 0xF" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_dsb(void) { __asm volatile("dsb 0xF" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_dmb(void) { __asm volatile("dmb 0xF" ::: "memory"); }

ARMM_ALWAYS_INLINE uint32_t armm_get_apsr(void) { uint32_t v; __asm volatile("mrs %0, apsr" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE uint32_t armm_get_ipsr(void) { uint32_t v; __asm volatile("mrs %0, ipsr" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE uint32_t armm_get_xpsr(void) { uint32_t v; __asm volatile("mrs %0, xpsr" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE uint32_t armm_get_control(void) { uint32_t v; __asm volatile("mrs %0, control" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_control(uint32_t v) { __asm volatile("msr control, %0" :: "r"(v) : "memory"); armm_isb(); }
ARMM_ALWAYS_INLINE uint32_t armm_get_primask(void) { uint32_t v; __asm volatile("mrs %0, primask" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_primask(uint32_t v) { __asm volatile("msr primask, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_msp(void) { uint32_t v; __asm volatile("mrs %0, msp" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_msp(uint32_t v) { __asm volatile("msr msp, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_psp(void) { uint32_t v; __asm volatile("mrs %0, psp" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_psp(uint32_t v) { __asm volatile("msr psp, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE void armm_irq_disable(void) { __asm volatile("cpsid i" ::: "memory"); }
ARMM_ALWAYS_INLINE void armm_irq_enable(void) { __asm volatile("cpsie i" ::: "memory"); }

#if ARMM_HAS_BASEPRI
ARMM_ALWAYS_INLINE uint32_t armm_get_basepri(void) { uint32_t v; __asm volatile("mrs %0, basepri" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_basepri(uint32_t v) { __asm volatile("msr basepri, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE void armm_set_basepri_max(uint32_t v) { __asm volatile("msr basepri_max, %0" :: "r"(v) : "memory"); }
#endif

#if ARMM_HAS_FAULTMASK
ARMM_ALWAYS_INLINE uint32_t armm_get_faultmask(void) { uint32_t v; __asm volatile("mrs %0, faultmask" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_faultmask(uint32_t v) { __asm volatile("msr faultmask, %0" :: "r"(v) : "memory"); }
#endif

#if ARMM_HAS_STACK_LIMIT
ARMM_ALWAYS_INLINE uint32_t armm_get_msplim(void) { uint32_t v; __asm volatile("mrs %0, msplim" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_msplim(uint32_t v) { __asm volatile("msr msplim, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_psplim(void) { uint32_t v; __asm volatile("mrs %0, psplim" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_psplim(uint32_t v) { __asm volatile("msr psplim, %0" :: "r"(v) : "memory"); }
#endif

#if ARMM_SECURE_BUILD
ARMM_ALWAYS_INLINE uint32_t armm_get_msp_ns(void) { uint32_t v; __asm volatile("mrs %0, msp_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_msp_ns(uint32_t v) { __asm volatile("msr msp_ns, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_psp_ns(void) { uint32_t v; __asm volatile("mrs %0, psp_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_psp_ns(uint32_t v) { __asm volatile("msr psp_ns, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_control_ns(void) { uint32_t v; __asm volatile("mrs %0, control_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_control_ns(uint32_t v) { __asm volatile("msr control_ns, %0" :: "r"(v) : "memory"); armm_isb(); }
ARMM_ALWAYS_INLINE uint32_t armm_get_primask_ns(void) { uint32_t v; __asm volatile("mrs %0, primask_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_primask_ns(uint32_t v) { __asm volatile("msr primask_ns, %0" :: "r"(v) : "memory"); }
# if ARMM_HAS_BASEPRI
ARMM_ALWAYS_INLINE uint32_t armm_get_basepri_ns(void) { uint32_t v; __asm volatile("mrs %0, basepri_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_basepri_ns(uint32_t v) { __asm volatile("msr basepri_ns, %0" :: "r"(v) : "memory"); }
# endif
# if ARMM_HAS_FAULTMASK
ARMM_ALWAYS_INLINE uint32_t armm_get_faultmask_ns(void) { uint32_t v; __asm volatile("mrs %0, faultmask_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_faultmask_ns(uint32_t v) { __asm volatile("msr faultmask_ns, %0" :: "r"(v) : "memory"); }
# endif
# if ARMM_HAS_STACK_LIMIT
ARMM_ALWAYS_INLINE uint32_t armm_get_msplim_ns(void) { uint32_t v; __asm volatile("mrs %0, msplim_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_msplim_ns(uint32_t v) { __asm volatile("msr msplim_ns, %0" :: "r"(v) : "memory"); }
ARMM_ALWAYS_INLINE uint32_t armm_get_psplim_ns(void) { uint32_t v; __asm volatile("mrs %0, psplim_ns" : "=r"(v)); return v; }
ARMM_ALWAYS_INLINE void armm_set_psplim_ns(uint32_t v) { __asm volatile("msr psplim_ns, %0" :: "r"(v) : "memory"); }
# endif
#endif

ARMM_ALWAYS_INLINE uint32_t armm_rbit(uint32_t v) { uint32_t r; __asm("rbit %0, %1" : "=r"(r) : "r"(v)); return r; }
ARMM_ALWAYS_INLINE uint32_t armm_rev(uint32_t v) { uint32_t r; __asm("rev %0, %1" : "=r"(r) : "r"(v)); return r; }
ARMM_ALWAYS_INLINE uint32_t armm_rev16(uint32_t v) { uint32_t r; __asm("rev16 %0, %1" : "=r"(r) : "r"(v)); return r; }
ARMM_ALWAYS_INLINE uint32_t armm_clz(uint32_t v) { return v ? (uint32_t)__builtin_clz(v) : 32u; }

#if !ARMM_ARCH_V6M
ARMM_ALWAYS_INLINE uint32_t armm_ldrex32(volatile uint32_t *p) { uint32_t v; __asm volatile("ldrex %0, [%1]" : "=&r"(v) : "r"(p) : "memory"); return v; }
ARMM_ALWAYS_INLINE uint32_t armm_strex32(uint32_t v, volatile uint32_t *p) { uint32_t s; __asm volatile("strex %0, %2, [%1]" : "=&r"(s) : "r"(p), "r"(v) : "memory"); return s; }
ARMM_ALWAYS_INLINE void armm_clrex(void) { __asm volatile("clrex" ::: "memory"); }
#endif

#endif
