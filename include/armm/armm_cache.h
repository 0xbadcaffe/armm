#ifndef ARMM_CACHE_H
#define ARMM_CACHE_H
#include "armm_scb.h"
#include <stddef.h>

/* Cache presence is implementation-defined. Override ARMM_HAS_ICACHE/DCACHE. */
#ifndef ARMM_HAS_ICACHE
# if defined(__ICACHE_PRESENT)
#  define ARMM_HAS_ICACHE (__ICACHE_PRESENT + 0)
# else
#  define ARMM_HAS_ICACHE 0
# endif
#endif
#ifndef ARMM_HAS_DCACHE
# if defined(__DCACHE_PRESENT)
#  define ARMM_HAS_DCACHE (__DCACHE_PRESENT + 0)
# else
#  define ARMM_HAS_DCACHE 0
# endif
#endif
#ifndef ARMM_DCACHE_LINE_SIZE
#define ARMM_DCACHE_LINE_SIZE 32u
#endif

#define ARMM_SCB_ICIALLU   ARMM_REG32(ARMM_SCB_BASE + 0x250UL)
#define ARMM_SCB_ICIMVAU   ARMM_REG32(ARMM_SCB_BASE + 0x258UL)
#define ARMM_SCB_DCIMVAC   ARMM_REG32(ARMM_SCB_BASE + 0x25CUL)
#define ARMM_SCB_DCCMVAU   ARMM_REG32(ARMM_SCB_BASE + 0x264UL)
#define ARMM_SCB_DCCMVAC   ARMM_REG32(ARMM_SCB_BASE + 0x268UL)
#define ARMM_SCB_DCCIMVAC  ARMM_REG32(ARMM_SCB_BASE + 0x270UL)
#define ARMM_CCR_DC        (1UL << 16)
#define ARMM_CCR_IC        (1UL << 17)

#if ARMM_HAS_ICACHE
ARMM_ALWAYS_INLINE void armm_icache_invalidate_all(void) { armm_dsb(); ARMM_SCB_ICIALLU = 0u; armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_icache_enable(void) { if ((ARMM_SCB_CCR & ARMM_CCR_IC) == 0u) { armm_icache_invalidate_all(); ARMM_SCB_CCR |= ARMM_CCR_IC; armm_dsb(); armm_isb(); } }
ARMM_ALWAYS_INLINE void armm_icache_disable(void) { armm_dsb(); armm_isb(); ARMM_SCB_CCR &= ~ARMM_CCR_IC; armm_icache_invalidate_all(); }
ARMM_ALWAYS_INLINE bool armm_icache_enabled(void) { return (ARMM_SCB_CCR & ARMM_CCR_IC) != 0u; }
#endif

#if ARMM_HAS_DCACHE
ARMM_ALWAYS_INLINE uintptr_t armm_cache_align_down(uintptr_t p) { return p & ~((uintptr_t)ARMM_DCACHE_LINE_SIZE - 1u); }
ARMM_ALWAYS_INLINE uintptr_t armm_cache_align_up(uintptr_t p) { return (p + ARMM_DCACHE_LINE_SIZE - 1u) & ~((uintptr_t)ARMM_DCACHE_LINE_SIZE - 1u); }
ARMM_ALWAYS_INLINE void armm_dcache_clean_range(const void *addr, size_t size)
{
    if (size == 0u) return;
    uintptr_t p = armm_cache_align_down((uintptr_t)addr), end = armm_cache_align_up((uintptr_t)addr + size);
    armm_dsb(); for (; p < end; p += ARMM_DCACHE_LINE_SIZE) ARMM_SCB_DCCMVAC = (uint32_t)p; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_dcache_invalidate_range(const void *addr, size_t size)
{
    if (size == 0u) return;
    uintptr_t p = armm_cache_align_down((uintptr_t)addr), end = armm_cache_align_up((uintptr_t)addr + size);
    armm_dsb(); for (; p < end; p += ARMM_DCACHE_LINE_SIZE) ARMM_SCB_DCIMVAC = (uint32_t)p; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_dcache_clean_invalidate_range(const void *addr, size_t size)
{
    if (size == 0u) return;
    uintptr_t p = armm_cache_align_down((uintptr_t)addr), end = armm_cache_align_up((uintptr_t)addr + size);
    armm_dsb(); for (; p < end; p += ARMM_DCACHE_LINE_SIZE) ARMM_SCB_DCCIMVAC = (uint32_t)p; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE bool armm_dcache_enabled(void) { return (ARMM_SCB_CCR & ARMM_CCR_DC) != 0u; }
/* Enable assumes cache RAM is already invalidated by reset/startup or platform code. */
ARMM_ALWAYS_INLINE void armm_dcache_enable_raw(void) { armm_dsb(); ARMM_SCB_CCR |= ARMM_CCR_DC; armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_dcache_disable_raw(void) { armm_dsb(); ARMM_SCB_CCR &= ~ARMM_CCR_DC; armm_dsb(); armm_isb(); }
#endif
#endif
