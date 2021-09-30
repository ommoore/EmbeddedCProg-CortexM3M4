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
Section11_Interrupt_priority_config.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Section11_Interrupt_priority_config.list \

OBJCOPY_BIN += \
Section11_Interrupt_priority_config.bin \


# All Target
all: Section11_Interrupt_priority_config.elf secondary-outputs

# Tool invocations
Section11_Interrupt_priority_config.elf: $(OBJS) $(USER_OBJS) C:\Users\Oliver\Desktop\FastBit\ Embedded\ Brain\ Academy\Embedded\ Systems\ Programming\ on\ ARM\ Cortex-M3M4\cortexmx\Section11_Interrupt_priority_config\STM32F446RETX_FLASH.ld
	arm-none-eabi-gcc -o "Section11_Interrupt_priority_config.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Oliver\Desktop\FastBit Embedded Brain Academy\Embedded Systems Programming on ARM Cortex-M3M4\cortexmx\Section11_Interrupt_priority_config\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Section11_Interrupt_priority_config.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Section11_Interrupt_priority_config.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Section11_Interrupt_priority_config.list"
	@echo 'Finished building: $@'
	@echo ' '

Section11_Interrupt_priority_config.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Section11_Interrupt_priority_config.bin"
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
