#ifndef ARMM_CORE_H
#define ARMM_CORE_H

#include "armm_intrinsics.h"

typedef enum { ARMM_MODE_THREAD = 0, ARMM_MODE_HANDLER = 1 } armm_mode_t;
typedef enum { ARMM_PRIVILEGED = 0, ARMM_UNPRIVILEGED = 1 } armm_privilege_t;
typedef enum { ARMM_STACK_MSP = 0, ARMM_STACK_PSP = 1 } armm_stack_t;

ARMM_ALWAYS_INLINE armm_mode_t armm_mode(void) { return (armm_get_ipsr() & 0x1FFu) ? ARMM_MODE_HANDLER : ARMM_MODE_THREAD; }
ARMM_ALWAYS_INLINE bool armm_in_thread_mode(void) { return armm_mode() == ARMM_MODE_THREAD; }
ARMM_ALWAYS_INLINE bool armm_in_handler_mode(void) { return armm_mode() == ARMM_MODE_HANDLER; }
ARMM_ALWAYS_INLINE uint32_t armm_exception_number(void) { return armm_get_ipsr() & 0x1FFu; }
ARMM_ALWAYS_INLINE armm_privilege_t armm_privilege(void) { return (armm_get_control() & 1u) ? ARMM_UNPRIVILEGED : ARMM_PRIVILEGED; }
ARMM_ALWAYS_INLINE armm_stack_t armm_thread_stack(void) { return (armm_get_control() & 2u) ? ARMM_STACK_PSP : ARMM_STACK_MSP; }
ARMM_ALWAYS_INLINE bool armm_fp_context_active(void) { return (armm_get_control() & 4u) != 0u; }

ARMM_ALWAYS_INLINE uint32_t armm_critical_enter(void)
{
    uint32_t state = armm_get_primask();
    armm_irq_disable();
    armm_dmb();
    return state;
}

ARMM_ALWAYS_INLINE void armm_critical_exit(uint32_t state)
{
    armm_dmb();
    armm_set_primask(state);
}

#endif
