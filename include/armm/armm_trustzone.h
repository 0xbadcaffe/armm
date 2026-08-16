#ifndef ARMM_TRUSTZONE_H
#define ARMM_TRUSTZONE_H
#include "armm_intrinsics.h"
#include <stddef.h>

#if ARMM_SECURE_BUILD
# if defined(__has_include)
#  if __has_include(<arm_cmse.h>)
#   include <arm_cmse.h>
#   define ARMM_HAS_CMSE_HEADER 1
#  endif
# endif
# ifndef ARMM_HAS_CMSE_HEADER
#  define ARMM_HAS_CMSE_HEADER 0
# endif

#define ARMM_SAU_BASE       (ARMM_SCS_BASE + 0x0DD0UL)
#define ARMM_SAU_CTRL       ARMM_REG32(ARMM_SAU_BASE + 0x00UL)
#define ARMM_SAU_TYPE       ARMM_REG32(ARMM_SAU_BASE + 0x04UL)
#define ARMM_SAU_RNR        ARMM_REG32(ARMM_SAU_BASE + 0x08UL)
#define ARMM_SAU_RBAR       ARMM_REG32(ARMM_SAU_BASE + 0x0CUL)
#define ARMM_SAU_RLAR       ARMM_REG32(ARMM_SAU_BASE + 0x10UL)
#define ARMM_SAU_CTRL_ENABLE (1UL << 0)
#define ARMM_SAU_CTRL_ALLNS  (1UL << 1)
#define ARMM_SAU_RLAR_NSC    (1UL << 1)
#define ARMM_SAU_RLAR_ENABLE (1UL << 0)

#define ARMM_SCB_NS_BASE       (ARMM_SCS_NS_BASE + 0x0D00UL)
#define ARMM_SCB_NS_VTOR       ARMM_REG32(ARMM_SCB_NS_BASE + 0x008UL)
#define ARMM_SCB_NS_AIRCR      ARMM_REG32(ARMM_SCB_NS_BASE + 0x00CUL)
#define ARMM_SCB_NS_SCR        ARMM_REG32(ARMM_SCB_NS_BASE + 0x010UL)

#define ARMM_NONSECURE_ENTRY __attribute__((cmse_nonsecure_entry))
#define ARMM_NONSECURE_CALL  __attribute__((cmse_nonsecure_call))

typedef enum { ARMM_SAU_NONSECURE = 0, ARMM_SAU_NSC = 1 } armm_sau_region_kind_t;

ARMM_ALWAYS_INLINE uint32_t armm_sau_region_count(void) { return ARMM_SAU_TYPE & 0xFFu; }
ARMM_ALWAYS_INLINE void armm_sau_disable(void) { ARMM_SAU_CTRL = 0u; armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_sau_enable(bool all_nonsecure_default)
{
    ARMM_SAU_CTRL = ARMM_SAU_CTRL_ENABLE | (all_nonsecure_default ? ARMM_SAU_CTRL_ALLNS : 0u); armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE bool armm_sau_region_set(uint32_t region, uintptr_t base, uintptr_t limit, armm_sau_region_kind_t kind)
{
    if ((base & 31u) != 0u || (limit & 31u) != 31u || base > limit) return false;
    ARMM_SAU_RNR = region;
    ARMM_SAU_RBAR = (uint32_t)(base & ~31u);
    ARMM_SAU_RLAR = (uint32_t)(limit & ~31u) | ARMM_SAU_RLAR_ENABLE | (kind == ARMM_SAU_NSC ? ARMM_SAU_RLAR_NSC : 0u);
    armm_dsb(); armm_isb(); return true;
}
ARMM_ALWAYS_INLINE void armm_sau_region_disable(uint32_t region)
{
    ARMM_SAU_RNR = region; ARMM_SAU_RLAR &= ~ARMM_SAU_RLAR_ENABLE; armm_dsb(); armm_isb();
}
ARMM_ALWAYS_INLINE void armm_ns_vector_table_set(uintptr_t addr) { ARMM_SCB_NS_VTOR = (uint32_t)addr; armm_dsb(); armm_isb(); }

# if ARMM_HAS_CMSE_HEADER
ARMM_ALWAYS_INLINE bool armm_cmse_range_readable(const void *p, size_t n)
{
    return cmse_check_address_range((void *)p, n, CMSE_NONSECURE | CMSE_MPU_READ) != NULL;
}
ARMM_ALWAYS_INLINE bool armm_cmse_range_readwrite(void *p, size_t n)
{
    return cmse_check_address_range(p, n, CMSE_NONSECURE | CMSE_MPU_READWRITE) != NULL;
}
# endif

/* For non-secure function pointers, prefer compiler CMSE built-ins. */
#define ARMM_NS_FUNC_TYPE(ret_, name_, args_) typedef ret_ (*name_) args_ ARMM_NONSECURE_CALL

#endif /* ARMM_SECURE_BUILD */
#endif
