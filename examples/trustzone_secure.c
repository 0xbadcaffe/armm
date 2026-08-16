#include "armm.h"

#if !ARMM_SECURE_BUILD
#error "Build this example as a Secure CMSE image, e.g. -mcpu=cortex-m33 -mcmse"
#endif

ARMM_NS_FUNC_TYPE(uint32_t, armm_ns_callback_t, (uint32_t));

ARMM_NONSECURE_ENTRY
uint32_t secure_add_one(uint32_t value)
{
    return value + 1u;
}

void secure_partition_example(void)
{
    armm_sau_disable();

    /* 0x20040000..0x2004FFFF -> Non-secure SRAM example. */
    (void)armm_sau_region_set(0u, 0x20040000u, 0x2004FFFFu,
                              ARMM_SAU_NONSECURE);

    /* Example NSC veneer window. Real addresses are linker/device specific. */
    (void)armm_sau_region_set(1u, 0x1003F000u, 0x1003FFFFu,
                              ARMM_SAU_NSC);

    armm_sau_enable(false);
}
