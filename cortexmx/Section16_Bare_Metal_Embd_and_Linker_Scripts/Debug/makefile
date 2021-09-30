################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Section16_Bare_Metal_Embd_and_Linker_Scripts.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Section16_Bare_Metal_Embd_and_Linker_Scripts.list \

OBJCOPY_BIN += \
Section16_Bare_Metal_Embd_and_Linker_Scripts.bin \


# All Target
all: Section16_Bare_Metal_Embd_and_Linker_Scripts.elf secondary-outputs

# Tool invocations
Section16_Bare_Metal_Embd_and_Linker_Scripts.elf: $(OBJS) $(USER_OBJS) C:\Users\Oliver\Desktop\FastBit\ Embedded\ Brain\ Academy\Embedded\ Systems\ Programming\ on\ ARM\ Cortex-M3M4\cortexmx\Section16_Bare_Metal_Embd_and_Linker_Scripts\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "Section16_Bare_Metal_Embd_and_Linker_Scripts.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Oliver\Desktop\FastBit Embedded Brain Academy\Embedded Systems Programming on ARM Cortex-M3M4\cortexmx\Section16_Bare_Metal_Embd_and_Linker_Scripts\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Section16_Bare_Metal_Embd_and_Linker_Scripts.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Section16_Bare_Metal_Embd_and_Linker_Scripts.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Section16_Bare_Metal_Embd_and_Linker_Scripts.list"
	@echo 'Finished building: $@'
	@echo ' '

Section16_Bare_Metal_Embd_and_Linker_Scripts.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Section16_Bare_Metal_Embd_and_Linker_Scripts.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
