CC ?= clang
TARGET_FLAGS ?= --target=arm-none-eabi -mcpu=cortex-m4 -mthumb
CFLAGS ?= -ffreestanding -std=c11 -Wall -Wextra -Werror -Iinclude
BUILD := build

.PHONY: all clean clang-matrix clang-trustzone gcc-m4 gcc-trustzone

all: $(BUILD)/smoke.o

$(BUILD):
	mkdir -p $@

$(BUILD)/smoke.o: tests/compile_smoke.c | $(BUILD)
	$(CC) $(TARGET_FLAGS) $(CFLAGS) -c $< -o $@

clang-matrix: | $(BUILD)
	@set -e; \
	for cpu in cortex-m0 cortex-m0plus cortex-m1 cortex-m3 cortex-m4 cortex-m7 cortex-m23 cortex-m33 cortex-m35p cortex-m52 cortex-m55 cortex-m85; do \
	  echo "[clang] $$cpu"; \
	  clang --target=arm-none-eabi -mcpu=$$cpu -mthumb $(CFLAGS) -c tests/compile_smoke.c -o $(BUILD)/$$cpu.o; \
	done

clang-trustzone: | $(BUILD)
	clang --target=arm-none-eabi -mcpu=cortex-m33 -mthumb -mcmse $(CFLAGS) -c tests/compile_smoke.c -o $(BUILD)/m33-secure-smoke.o
	clang --target=arm-none-eabi -mcpu=cortex-m33 -mthumb -mcmse $(CFLAGS) -c examples/trustzone_secure.c -o $(BUILD)/m33-secure-example.o

gcc-m4: | $(BUILD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb $(CFLAGS) -c tests/compile_smoke.c -o $(BUILD)/gcc-m4.o

gcc-trustzone: | $(BUILD)
	arm-none-eabi-gcc -mcpu=cortex-m33 -mthumb -mcmse $(CFLAGS) -c examples/trustzone_secure.c -o $(BUILD)/gcc-m33-secure.o

clean:
	rm -rf $(BUILD)
