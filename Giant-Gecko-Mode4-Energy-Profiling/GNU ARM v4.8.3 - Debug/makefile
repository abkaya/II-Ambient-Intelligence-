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
-include CMSIS/efm32gg/subdir.mk
-include BSP/subdir.mk
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
all: Giant-Gecko-Mode4-Energy-Profiling.axf

# Tool invocations
Giant-Gecko-Mode4-Energy-Profiling.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "Giant-Gecko-Mode4-Energy-Profiling.ld" -Xlinker --gc-sections -Xlinker -Map="Giant-Gecko-Mode4-Energy-Profiling.map" --specs=nano.specs -o Giant-Gecko-Mode4-Energy-Profiling.axf "./src/emodes.o" "./src/main.o" "./emlib/em_assert.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_int.o" "./emlib/em_lcd.o" "./emlib/em_rtc.o" "./emlib/em_system.o" "./Drivers/rtcdriver.o" "./Drivers/segmentlcd.o" "./CMSIS/efm32gg/startup_efm32gg.o" "./CMSIS/efm32gg/system_efm32gg.o" "./BSP/bsp_trace.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: Giant-Gecko-Mode4-Energy-Profiling.hex'
	arm-none-eabi-objcopy -O ihex "Giant-Gecko-Mode4-Energy-Profiling.axf" "Giant-Gecko-Mode4-Energy-Profiling.hex"
	@echo ' '

	@echo 'Building bin file: Giant-Gecko-Mode4-Energy-Profiling.bin'
	arm-none-eabi-objcopy -O binary "Giant-Gecko-Mode4-Energy-Profiling.axf" "Giant-Gecko-Mode4-Energy-Profiling.bin"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "Giant-Gecko-Mode4-Energy-Profiling.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Giant-Gecko-Mode4-Energy-Profiling.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
