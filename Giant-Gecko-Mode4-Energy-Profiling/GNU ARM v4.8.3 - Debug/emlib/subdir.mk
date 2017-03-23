################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_assert.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_cmu.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_emu.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_gpio.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_int.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_lcd.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_rtc.c \
/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_system.c 

OBJS += \
./emlib/em_assert.o \
./emlib/em_cmu.o \
./emlib/em_emu.o \
./emlib/em_gpio.o \
./emlib/em_int.o \
./emlib/em_lcd.o \
./emlib/em_rtc.o \
./emlib/em_system.o 

C_DEPS += \
./emlib/em_assert.d \
./emlib/em_cmu.d \
./emlib/em_emu.d \
./emlib/em_gpio.d \
./emlib/em_int.d \
./emlib/em_lcd.d \
./emlib/em_rtc.d \
./emlib/em_system.d 


# Each subdirectory must supply rules for building sources it contributes
emlib/em_assert.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_assert.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_assert.d" -MT"emlib/em_assert.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_cmu.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_cmu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_cmu.d" -MT"emlib/em_cmu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_emu.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_emu.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_emu.d" -MT"emlib/em_emu.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_gpio.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_gpio.d" -MT"emlib/em_gpio.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_int.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_int.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_int.d" -MT"emlib/em_int.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_lcd.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_lcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_lcd.d" -MT"emlib/em_lcd.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_rtc.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_rtc.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_rtc.d" -MT"emlib/em_rtc.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

emlib/em_system.o: /home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/src/em_system.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DDEBUG=1' -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/examples/emode" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/EFM32GG_STK3700/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/CMSIS/Include" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emlib/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/common/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/config" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/dmadrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/gpiointerrupt/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/nvm/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/rtcdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/sleep/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/spidrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/uartdrv/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/emdrv/ustimer/inc" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/bsp" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/kits/common/drivers" -I"/home/clnx/SimplicityStudio/SimplicityStudio_v3/developer/sdks/efm32/v2/Device/SiliconLabs/EFM32GG/Include" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99 -MMD -MP -MF"emlib/em_system.d" -MT"emlib/em_system.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


