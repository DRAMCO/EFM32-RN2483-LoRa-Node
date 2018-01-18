################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32HG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EFM32-RN2483-LoRa-Node.axf

# Tool invocations
EFM32-RN2483-LoRa-Node.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -T "EFM32-RN2483-LoRa-Node.ld" -Xlinker --gc-sections -Xlinker -Map="EFM32-RN2483-LoRa-Node.map" --specs=nano.specs -o EFM32-RN2483-LoRa-Node.axf "./CMSIS/EFM32HG/startup_efm32hg.o" "./CMSIS/EFM32HG/system_efm32hg.o" "./Drivers/i2cspm.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./src/bme280.o" "./src/delay.o" "./src/main.o" "./src/usart.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: EFM32-RN2483-LoRa-Node.hex'
	arm-none-eabi-objcopy -O ihex "EFM32-RN2483-LoRa-Node.axf" "EFM32-RN2483-LoRa-Node.hex"
	@echo ' '

	@echo 'Building bin file: EFM32-RN2483-LoRa-Node.bin'
	arm-none-eabi-objcopy -O binary "EFM32-RN2483-LoRa-Node.axf" "EFM32-RN2483-LoRa-Node.bin"
	@echo ' '

	@echo 'Building s37 file: EFM32-RN2483-LoRa-Node.s37'
	arm-none-eabi-objcopy -O srec "EFM32-RN2483-LoRa-Node.axf" "EFM32-RN2483-LoRa-Node.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "EFM32-RN2483-LoRa-Node.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) EFM32-RN2483-LoRa-Node.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
