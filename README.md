# cm4u — Cortex-M4 Core Utilities

```text
                .---------------------------.
               /      C O R T E X  - M 4    \
              /-------------------------------\
             |   __  __   _  _   _   _        |
             |  |  \/  | | || | | | | |       |
             |  | |\/| | | || |_| | | |       |
             |  | |  | | |__   _| | |_| |      |
             |  |_|  |_|    |_|    \___/       |
             |                                 |
             |    [ NVIC ] [ SCB ] [ DWT ]    |
             |    [ MPU  ] [ FPU ] [ SysTick ]|
             |          register-level         |
              \_______________________________/
                    \   ^__^   /
                     \  (oo)  /
                        (__)       bare metal
```

`cm4u.h` is a small, header-only Cortex-M4 core layer intended to sit below an
embedded application or vendor peripheral driver layer.

It deliberately does **not** include CMSIS and does not depend on a vendor HAL.
The API is implemented with Cortex-M4 system-register mappings plus compiler
intrinsics/inline assembly.

## Scope

- Core special registers: APSR, IPSR, xPSR, CONTROL, PRIMASK, BASEPRI,
  FAULTMASK, MSP, PSP
- Thread/Handler mode, privilege and active stack inspection
- PRIMASK and BASEPRI critical sections
- CPU instructions: NOP, WFI, WFE, SEV, ISB, DSB, DMB, CLREX
- Exclusive accesses: LDREX/STREX 8/16/32
- NVIC enable/disable/pending/active/priority/priority grouping/software trigger
- SCB exception state, PendSV, SysTick pending, NMI, VTOR, sleep control,
  divide-by-zero and unaligned traps
- Configurable fault enable and fault status/address access
- System reset
- SysTick setup and direct control
- DWT cycle counter, profiling, event counters, comparator configuration,
  microsecond/millisecond delay helpers
- CoreDebug trace and DebugMonitor helpers
- Armv7-M MPU region control
- Cortex-M4 FPU enable/lazy-stacking/status helpers
- Basic ITM stimulus output
- Small LDREX/STREX atomics
- Bit helpers: byte reverse, bit reverse, CLZ, popcount, rotate, saturation

## Usage

```c
#include "cm4u.h"

int main(void)
{
    const uint32_t core_hz = 168000000u;

    cm4u_fpu_enable();
    cm4u_configurable_faults_enable();
    cm4u_div0_trap_enable();
    cm4u_dwt_init();

    cm4u_delay_us(10u, core_hz);

    if (cm4u_in_thread_mode()) {
        /* normal Thread mode */
    }

    for (;;) {
        cm4u_wfi();
    }
}
```

## NVIC priority width

The number of implemented NVIC priority bits is device-specific. Set it before
including the header if your MCU does not implement 4 bits:

```c
#define CM4U_NVIC_PRIO_BITS 3u
#include "cm4u.h"
```

## MPU note

The MPU is optional in the architecture. `cm4u_mpu_region_count()` lets you
inspect the implemented region count at runtime. Region size must be a power of
two and at least 32 bytes for the PMSAv7 MPU.

## FPU note

Some Cortex-M4 implementations are Cortex-M4F (FPU present) and some do not
implement the FPU. Set `CM4U_HAS_FPU=0` in a platform configuration if you want
to exclude FPU use at a higher layer. The current header keeps the register API
available because it targets the architectural Cortex-M4 core map.

## Design boundary

This is a **core** layer, not a complete MCU HAL. It intentionally does not own:

- RCC/clock-tree setup
- GPIO
- DMA
- UART/SPI/I2C
- ADC/DAC
- vendor interrupt numbers
- vendor-specific power/reset registers

Those belong in a small device/platform module layered on top of `cm4u.h`.

## Compatibility philosophy

The API mirrors the useful functional surface of CMSIS-Core for Cortex-M4,
while exposing extra convenience helpers such as mode checks, DWT delays,
profiling, fault inspection and atomics. Names are intentionally different so
it can coexist with CMSIS during migration.
