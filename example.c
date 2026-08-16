#include "cm4u.h"

#ifndef CORE_CLOCK_HZ
#define CORE_CLOCK_HZ 168000000u
#endif

volatile uint32_t demo_counter;

int main(void)
{
    cm4u_configurable_faults_enable();
    cm4u_div0_trap_enable();
    cm4u_unalign_trap_enable();
    (void)cm4u_dwt_init();

    uint32_t start = cm4u_profile_begin();
    cm4u_delay_us(10u, CORE_CLOCK_HZ);
    uint32_t elapsed = cm4u_profile_end(start);
    (void)elapsed;

    uint32_t old = cm4u_atomic_fetch_add32(&demo_counter, 1u);
    (void)old;

    for (;;) {
        cm4u_wfi();
    }
}
