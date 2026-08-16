#ifndef ARMM_NVIC_H
#define ARMM_NVIC_H

#include "armm_intrinsics.h"

#define ARMM_NVIC_BASE          (ARMM_SCS_BASE + 0x0100UL)
#define ARMM_NVIC_ISER(n_)      ARMM_REG32(ARMM_NVIC_BASE + 0x000UL + 4UL*(n_))
#define ARMM_NVIC_ICER(n_)      ARMM_REG32(ARMM_NVIC_BASE + 0x080UL + 4UL*(n_))
#define ARMM_NVIC_ISPR(n_)      ARMM_REG32(ARMM_NVIC_BASE + 0x100UL + 4UL*(n_))
#define ARMM_NVIC_ICPR(n_)      ARMM_REG32(ARMM_NVIC_BASE + 0x180UL + 4UL*(n_))
#define ARMM_NVIC_IABR(n_)      ARMM_REG32(ARMM_NVIC_BASE + 0x200UL + 4UL*(n_))
#define ARMM_NVIC_IPR(n_)       ARMM_REG8 (ARMM_NVIC_BASE + 0x300UL + (n_))
#define ARMM_NVIC_STIR          ARMM_REG32(ARMM_SCS_BASE + 0x0F00UL)

ARMM_ALWAYS_INLINE void armm_nvic_enable(uint32_t irq) { ARMM_NVIC_ISER(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE void armm_nvic_disable(uint32_t irq) { ARMM_NVIC_ICER(irq >> 5) = 1UL << (irq & 31u); armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_nvic_set_pending(uint32_t irq) { ARMM_NVIC_ISPR(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE void armm_nvic_clear_pending(uint32_t irq) { ARMM_NVIC_ICPR(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE bool armm_nvic_is_enabled(uint32_t irq) { return (ARMM_NVIC_ISER(irq >> 5) & (1UL << (irq & 31u))) != 0u; }
ARMM_ALWAYS_INLINE bool armm_nvic_is_pending(uint32_t irq) { return (ARMM_NVIC_ISPR(irq >> 5) & (1UL << (irq & 31u))) != 0u; }
ARMM_ALWAYS_INLINE bool armm_nvic_is_active(uint32_t irq) { return (ARMM_NVIC_IABR(irq >> 5) & (1UL << (irq & 31u))) != 0u; }
ARMM_ALWAYS_INLINE void armm_nvic_set_priority_raw(uint32_t irq, uint8_t raw) { ARMM_NVIC_IPR(irq) = raw; }
ARMM_ALWAYS_INLINE uint8_t armm_nvic_get_priority_raw(uint32_t irq) { return ARMM_NVIC_IPR(irq); }
ARMM_ALWAYS_INLINE void armm_nvic_set_priority(uint32_t irq, uint32_t priority) { ARMM_NVIC_IPR(irq) = (uint8_t)((priority << (8u - ARMM_CFG_NVIC_PRIO_BITS)) & 0xFFu); }
ARMM_ALWAYS_INLINE uint32_t armm_nvic_get_priority(uint32_t irq) { return (uint32_t)ARMM_NVIC_IPR(irq) >> (8u - ARMM_CFG_NVIC_PRIO_BITS); }
#if !ARMM_ARCH_V6M
ARMM_ALWAYS_INLINE void armm_nvic_trigger(uint32_t irq) { ARMM_NVIC_STIR = irq & 0x1FFu; }
#if ARMM_SECURE_BUILD
#define ARMM_NVIC_NS_BASE       (ARMM_SCS_NS_BASE + 0x0100UL)
#define ARMM_NVIC_NS_ISER(n_)   ARMM_REG32(ARMM_NVIC_NS_BASE + 0x000UL + 4UL*(n_))
#define ARMM_NVIC_NS_ICER(n_)   ARMM_REG32(ARMM_NVIC_NS_BASE + 0x080UL + 4UL*(n_))
#define ARMM_NVIC_NS_ISPR(n_)   ARMM_REG32(ARMM_NVIC_NS_BASE + 0x100UL + 4UL*(n_))
#define ARMM_NVIC_NS_ICPR(n_)   ARMM_REG32(ARMM_NVIC_NS_BASE + 0x180UL + 4UL*(n_))
#define ARMM_NVIC_NS_IPR(n_)    ARMM_REG8 (ARMM_NVIC_NS_BASE + 0x300UL + (n_))
ARMM_ALWAYS_INLINE void armm_nvic_ns_enable(uint32_t irq) { ARMM_NVIC_NS_ISER(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE void armm_nvic_ns_disable(uint32_t irq) { ARMM_NVIC_NS_ICER(irq >> 5) = 1UL << (irq & 31u); armm_dsb(); armm_isb(); }
ARMM_ALWAYS_INLINE void armm_nvic_ns_set_pending(uint32_t irq) { ARMM_NVIC_NS_ISPR(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE void armm_nvic_ns_clear_pending(uint32_t irq) { ARMM_NVIC_NS_ICPR(irq >> 5) = 1UL << (irq & 31u); }
ARMM_ALWAYS_INLINE void armm_nvic_ns_set_priority(uint32_t irq, uint32_t priority) { ARMM_NVIC_NS_IPR(irq) = (uint8_t)(priority << (8u - ARMM_CFG_NVIC_PRIO_BITS)); }
#endif

#endif

#endif
