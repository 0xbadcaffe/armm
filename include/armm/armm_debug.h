#ifndef ARMM_DEBUG_H
#define ARMM_DEBUG_H
#include "armm_intrinsics.h"

#define ARMM_COREDEBUG_DHCSR ARMM_REG32(ARMM_COREDEBUG_BASE + 0x00UL)
#define ARMM_COREDEBUG_DCRSR ARMM_REG32(ARMM_COREDEBUG_BASE + 0x04UL)
#define ARMM_COREDEBUG_DCRDR ARMM_REG32(ARMM_COREDEBUG_BASE + 0x08UL)
#define ARMM_COREDEBUG_DEMCR ARMM_REG32(ARMM_COREDEBUG_BASE + 0x0CUL)
#define ARMM_DHCSR_C_DEBUGEN (1UL << 0)
#define ARMM_DHCSR_S_HALT    (1UL << 17)
#define ARMM_ITM_TCR         ARMM_REG32(ARMM_ITM_BASE + 0xE80UL)
#define ARMM_ITM_TER         ARMM_REG32(ARMM_ITM_BASE + 0xE00UL)

ARMM_ALWAYS_INLINE bool armm_debugger_attached(void) { return (ARMM_COREDEBUG_DHCSR & ARMM_DHCSR_C_DEBUGEN) != 0u; }
ARMM_ALWAYS_INLINE bool armm_core_halted(void) { return (ARMM_COREDEBUG_DHCSR & ARMM_DHCSR_S_HALT) != 0u; }
ARMM_ALWAYS_INLINE bool armm_itm_port_enabled(uint32_t port)
{
    return port < 32u && (ARMM_ITM_TCR & 1u) && (ARMM_ITM_TER & (1UL << port));
}
ARMM_ALWAYS_INLINE bool armm_itm_write8(uint32_t port, uint8_t ch)
{
    if (!armm_itm_port_enabled(port)) return false;
    volatile uint32_t *stim32 = (volatile uint32_t *)(uintptr_t)(ARMM_ITM_BASE + 4UL * port);
    volatile uint8_t *stim8 = (volatile uint8_t *)(uintptr_t)(ARMM_ITM_BASE + 4UL * port);
    while (*stim32 == 0u) { }
    *stim8 = ch;
    return true;
}
#endif
