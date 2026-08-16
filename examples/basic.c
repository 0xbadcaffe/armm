#include "armm.h"

#ifndef ARMM_CORE_CLOCK_HZ
#define ARMM_CORE_CLOCK_HZ 48000000u
#endif

int main(void)
{
#if ARMM_HAS_DWT
    (void)armm_dwt_cycle_counter_enable();
    armm_delay_us(10u, ARMM_CORE_CLOCK_HZ);
#endif

    if (armm_in_thread_mode()) {
        uint32_t state = armm_critical_enter();
        /* atomic setup */
        armm_critical_exit(state);
    }

    for (;;) {
        armm_wfi();
    }
}
