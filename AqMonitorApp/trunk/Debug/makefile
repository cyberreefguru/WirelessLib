################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
AqMonitor.lss \

FLASH_IMAGE += \
AqMonitor.hex \

SIZEDUMMY += \
sizedummy \

AVRDUDEDUMMY += \
avrdudedummy \


# All Target
all: AqMonitor.elf secondary-outputs

# Tool invocations
AqMonitor.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc --cref -s -Os -o"AqMonitor.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm -Wl,-Map,AqMonitor.map,--cref -L"/Users/tom/Development/git/Arduino/Wire/Debug" -L"/Users/tom/Development/git/Arduino/SoftwareSerial/Debug" -L"/Users/tom/Development/git/Arduino/avrlib/Release" -mmcu=atmega328p
	@echo 'Finished building target: $@'
	@echo ' '

AqMonitor.lss: AqMonitor.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S AqMonitor.elf  >"AqMonitor.lss"
	@echo 'Finished building: $@'
	@echo ' '

AqMonitor.hex: AqMonitor.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex AqMonitor.elf  "AqMonitor.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: AqMonitor.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p AqMonitor.elf
	@echo 'Finished building: $@'
	@echo ' '

avrdudedummy: AqMonitor.elf
	@echo 'Invoking: AVRDude'
	/usr/local/CrossPack-AVR-20100115/bin/avrdude -pm328p -carduino -P/dev/tty.usbserial-A600aftj -b57600 -Uflash:w:AqMonitor.hex:a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_DEPS)$(ELFS)$(AVRDUDEDUMMY)$(S_UPPER_DEPS)$(C++_DEPS)$(LSS)$(S_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(OBJS)$(FLASH_IMAGE)$(C_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) AqMonitor.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY) $(AVRDUDEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
