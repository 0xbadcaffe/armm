```text
          _    ____  __  __ __  __
         / \  |  _ \|  \/  |  \/  |
        / _ \ | |_) | |\/| | |\/| |
       / ___ \|  _ <| |  | | |  | |
      /_/   \_\_| \_\_|  |_|_|  |_|

      A R M   M - P R O F I L E   R U N T I M E

       M0  M0+  M1  M3  M4  M7  M23  M33
             M35P  M52  M55  M85

        bare metal • header only • no CMSIS required
```

# armm 🦾

`armm` is a small, header-only Arm M-profile architectural runtime.

It is intentionally below a vendor HAL/BSP and narrower than CMSIS. It owns the
CPU/core-facing layer: special registers, exception state, NVIC, SCB, SysTick,
DWT, MPU, FPU control, cache maintenance, debug state, and Armv8-M TrustZone/SAU.

It does **not** provide GPIO, clocks, UART, ADC, DMA, vendor IRQ numbers or
SoC-specific security controllers.

## Supported architecture families

| Architecture | Typical Cortex-M cores | armm support |
|---|---|---|
| Armv6-M | M0, M0+, M1 | core/NVIC/SysTick/basic SCB |
| Armv7-M | M3 | mainline exceptions, DWT, v7 MPU |
| Armv7E-M | M4, M7 | above + DSP/FPU detection; cache helpers when present |
| Armv8-M Baseline | M23 | baseline core + TrustZone when CMSE enabled |
| Armv8-M Mainline | M33, M35P | stack limits, TrustZone/SAU, v8 MPU |
| Armv8.1-M Mainline | M52, M55, M85 | v8.1 feature detection including MVE |

Feature macros are capability based. Optional implementation features can be
overridden before including `armm.h`.

## Header layout

```text
include/
├── armm.h                 umbrella include
└── armm/
    ├── armm.h             version + module aggregation
    ├── armm_arch.h        architecture/capability detection
    ├── armm_intrinsics.h  instructions + special registers
    ├── armm_core.h        mode/privilege/stack/critical sections
    ├── armm_nvic.h        interrupt controller
    ├── armm_scb.h         SCB/reset/fault/vector helpers
    ├── armm_systick.h     SysTick
    ├── armm_dwt.h         cycle counter/profiling/delays
    ├── armm_mpu.h         Armv7-M and Armv8-M MPU programming models
    ├── armm_fpu.h         FPU control/lazy stacking
    ├── armm_cache.h       I-cache and D-cache maintenance helpers
    ├── armm_debug.h       CoreDebug + ITM helpers
    ├── armm_features.h    runtime-friendly feature queries
    └── armm_trustzone.h   SAU/CMSE/non-secure aliases
```

## Quick start

```c
#include "armm.h"

int main(void)
{
#if ARMM_HAS_DWT
    armm_dwt_cycle_counter_enable();
    armm_delay_us(10u, 168000000u);
#endif

    if (armm_in_thread_mode()) {
        uint32_t key = armm_critical_enter();
        /* critical work */
        armm_critical_exit(key);
    }

    for (;;) {
        armm_wfi();
    }
}
```

## Capability macros

Common macros include:

```c
ARMM_ARCH_V6M
ARMM_ARCH_V7M
ARMM_ARCH_V7EM
ARMM_ARCH_V8M_BASE
ARMM_ARCH_V8M_MAIN
ARMM_ARCH_V81M_MAIN

ARMM_HAS_BASEPRI
ARMM_HAS_FAULTMASK
ARMM_HAS_DWT
ARMM_HAS_MPU
ARMM_HAS_FPU
ARMM_HAS_DSP
ARMM_HAS_MVE
ARMM_HAS_TRUSTZONE
ARMM_HAS_SAU
ARMM_HAS_STACK_LIMIT
ARMM_HAS_ICACHE
ARMM_HAS_DCACHE
ARMM_SECURE_BUILD
```

For vendor/configuration-specific features you may override the default before
including the library, for example:

```c
#define ARMM_HAS_MPU 0
#define ARMM_HAS_DCACHE 0
#define ARMM_CFG_NVIC_PRIO_BITS 4u
#include "armm.h"
```

`ARMM_CFG_NVIC_PRIO_BITS` **must match the implemented NVIC priority width** of
your MCU if you use logical priority APIs.

## TrustZone / CMSE

TrustZone APIs are enabled only for a Secure CMSE build. With GCC or Clang this
means using `-mcmse` on a core that implements the Armv8-M Security Extension.

```c
#include "armm.h"

ARMM_NONSECURE_ENTRY
uint32_t secure_service(uint32_t x)
{
    return x + 1u;
}

void security_init(void)
{
    armm_sau_disable();

    armm_sau_region_set(0,
                        0x20040000u,
                        0x2004FFFFu,
                        ARMM_SAU_NONSECURE);

    armm_sau_enable(false);
}
```

The SAU is architectural. Vendor-specific MPC/PPC/IDAU configuration stays
outside `armm`.

## Build with Clang

Compile a smoke test for Cortex-M4:

```bash
clang \
  --target=arm-none-eabi \
  -mcpu=cortex-m4 \
  -mthumb \
  -ffreestanding \
  -std=c11 \
  -Wall -Wextra -Werror \
  -Iinclude \
  -c tests/compile_smoke.c \
  -o build/m4.o
```

Cortex-M33 Secure/TrustZone build:

```bash
clang \
  --target=arm-none-eabi \
  -mcpu=cortex-m33 \
  -mthumb \
  -mcmse \
  -ffreestanding \
  -std=c11 \
  -Wall -Wextra -Werror \
  -Iinclude \
  -c examples/trustzone_secure.c \
  -o build/m33-secure.o
```

Example MVE-capable compile:

```bash
clang \
  --target=arm-none-eabi \
  -mcpu=cortex-m55 \
  -mthumb \
  -ffreestanding \
  -std=c11 \
  -Iinclude \
  -c tests/compile_smoke.c \
  -o build/m55.o
```

## Build with Arm GNU Toolchain (`arm-none-eabi-gcc`)

Cortex-M4:

```bash
arm-none-eabi-gcc \
  -mcpu=cortex-m4 \
  -mthumb \
  -ffreestanding \
  -std=c11 \
  -Wall -Wextra -Werror \
  -Iinclude \
  -c tests/compile_smoke.c \
  -o build/m4.o
```

Cortex-M33 Secure/TrustZone:

```bash
arm-none-eabi-gcc \
  -mcpu=cortex-m33 \
  -mthumb \
  -mcmse \
  -ffreestanding \
  -std=c11 \
  -Wall -Wextra -Werror \
  -Iinclude \
  -c examples/trustzone_secure.c \
  -o build/m33-secure.o
```

Cortex-M55:

```bash
arm-none-eabi-gcc \
  -mcpu=cortex-m55 \
  -mthumb \
  -ffreestanding \
  -std=c11 \
  -Iinclude \
  -c tests/compile_smoke.c \
  -o build/m55.o
```

For M52/M85, use a recent Arm GNU Toolchain release that recognizes those
`-mcpu=` names. If an older toolchain does not, upgrade it rather than lying to
the compiler about the target CPU.

## Makefile

```bash
make CC=clang TARGET_FLAGS="--target=arm-none-eabi -mcpu=cortex-m4 -mthumb"
make CC=arm-none-eabi-gcc TARGET_FLAGS="-mcpu=cortex-m4 -mthumb"

make clang-matrix
make clang-trustzone
make gcc-m4
make gcc-trustzone
```

`clang-matrix` compiles the smoke test for all named Cortex-M cores supported by
the installed Clang.

## Scope and safety notes

- This library does not know your vendor IRQ enumeration. NVIC functions accept
  the external IRQ number as an unsigned integer.
- MPU region helpers accept architectural register encodings intentionally; a
  higher-level policy builder can be layered on later without hiding the core.
- Cache presence and line size are implementation-specific. Override
  `ARMM_HAS_ICACHE`, `ARMM_HAS_DCACHE`, and `ARMM_DCACHE_LINE_SIZE` when needed.
- `armm_dcache_enable_raw()` is deliberately named `raw`: startup/platform code
  must ensure cache state is valid before enabling D-cache.
- DWT is not guaranteed on every implementation even when the architecture can
  support it; capability macros are overrideable for that reason.
- TrustZone memory partitioning requires correct linker scripts and usually
  vendor-specific security attribution outside the architectural SAU.

## Validation performed for this kit

The included headers were compiled with Clang 17 using `-Wall -Wextra -Werror`
for:

```text
cortex-m0
cortex-m0plus
cortex-m1
cortex-m3
cortex-m4
cortex-m7
cortex-m23
cortex-m33
cortex-m35p
cortex-m52
cortex-m55
cortex-m85
```

A Secure Cortex-M33 build was also compiled with `-mcmse`, including the
TrustZone example.

The build environment used to produce this kit did not contain
`arm-none-eabi-gcc`, so the GNU commands are provided but were not executed in
this packaging run.

## Architectural references

The implementation was cross-checked against Arm/CMSIS architectural material:

- Arm M-profile architectures: https://www.arm.com/architecture/cpu/m-profile
- CMSIS-Core TrustZone: https://arm-software.github.io/CMSIS_6/main/Core/using_TrustZone_pg.html
- CMSIS-Core NVIC: https://arm-software.github.io/CMSIS_6/latest/Core/group__NVIC__gr.html
- Arm GNU Toolchain: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

## License

MIT. See `LICENSE`.
