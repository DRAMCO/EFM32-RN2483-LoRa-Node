################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/adc.c \
../system/buttons.c \
../system/iic.c \
../system/irq.c \
../system/led.c \
../system/leuart.c \
../system/pm.c \
../system/system.c 

OBJS += \
./system/adc.o \
./system/buttons.o \
./system/iic.o \
./system/irq.o \
./system/led.o \
./system/leuart.o \
./system/pm.o \
./system/system.o 

C_DEPS += \
./system/adc.d \
./system/buttons.d \
./system/iic.d \
./system/irq.d \
./system/led.d \
./system/leuart.d \
./system/pm.d \
./system/system.d 


# Each subdirectory must supply rules for building sources it contributes
system/adc.o: ../system/adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/adc.d" -MT"system/adc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/buttons.o: ../system/buttons.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/buttons.d" -MT"system/buttons.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/iic.o: ../system/iic.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/iic.d" -MT"system/iic.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/irq.o: ../system/irq.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/irq.d" -MT"system/irq.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/led.o: ../system/led.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/led.d" -MT"system/led.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/leuart.o: ../system/leuart.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/leuart.d" -MT"system/leuart.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/pm.o: ../system/pm.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/pm.d" -MT"system/pm.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/system.o: ../system/system.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"system/system.d" -MT"system/system.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


