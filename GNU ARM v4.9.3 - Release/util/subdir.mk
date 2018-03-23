################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../util/delay.c \
../util/util.c 

OBJS += \
./util/delay.o \
./util/util.o 

C_DEPS += \
./util/delay.d \
./util/util.d 


# Each subdirectory must supply rules for building sources it contributes
util/delay.o: ../util/delay.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"util/delay.d" -MT"util/delay.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

util/util.o: ../util/util.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m0plus -mthumb -std=c99 '-DDEBUG=1' '-DEFM32HG322F64=1' '-DNDEBUG=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/rtcdrv/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emdrv/common/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/SLSTK3400A_EFM32HG/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/Device/SiliconLabs/EFM32HG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.2//hardware/kit/common/drivers" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\util" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\lora" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\sensors" -I"C:\Users\geoffrey\SimplicityStudio\v4_workspace\EFM32-RN2483-LoRa-Node\system" -O3 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -MMD -MP -MF"util/util.d" -MT"util/util.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


