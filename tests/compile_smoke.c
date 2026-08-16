#include "armm.h"

volatile uint32_t sink;

void smoke(void)
{
    sink = armm_get_ipsr();
    sink ^= armm_get_control();
    sink ^= armm_get_primask();
    sink ^= armm_get_msp();
    sink ^= armm_get_psp();
    sink ^= armm_exception_number();
    sink ^= armm_cpuid();
    armm_nop();
    armm_dmb();
    armm_dsb();
    armm_isb();
    armm_nvic_set_priority(0u, 1u);
    armm_systick_stop();
#if ARMM_HAS_BASEPRI
    sink ^= armm_get_basepri();
#endif
#if ARMM_HAS_FAULTMASK
    sink ^= armm_get_faultmask();
#endif
#if ARMM_HAS_DWT
    (void)armm_dwt_cycle_counter_enable();
    sink ^= armm_cycles();
#endif
#if ARMM_HAS_FPU
    armm_fpu_enable();
#endif
#if ARMM_HAS_STACK_LIMIT
    sink ^= armm_get_msplim();
#endif
#if ARMM_SECURE_BUILD
    sink ^= armm_get_msp_ns();
    sink ^= armm_sau_region_count();
#endif
}
