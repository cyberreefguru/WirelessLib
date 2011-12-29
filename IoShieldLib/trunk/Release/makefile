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
IoShieldLib.lss \

FLASH_IMAGE += \
IoShieldLib.hex \

EEPROM_IMAGE += \
IoShieldLib.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: IoShieldLib.elf secondary-outputs

# Tool invocations
IoShieldLib.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-g++ -Wl,-Map,IoShieldLib.map,--cref -mmcu=atmega328p -o"IoShieldLib.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

IoShieldLib.lss: IoShieldLib.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S IoShieldLib.elf  >"IoShieldLib.lss"
	@echo 'Finished building: $@'
	@echo ' '

IoShieldLib.hex: IoShieldLib.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex IoShieldLib.elf  "IoShieldLib.hex"
	@echo 'Finished building: $@'
	@echo ' '

IoShieldLib.eep: IoShieldLib.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex IoShieldLib.elf  "IoShieldLib.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: IoShieldLib.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p IoShieldLib.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_DEPS)$(ELFS)$(EEPROM_IMAGE)$(S_UPPER_DEPS)$(C++_DEPS)$(LSS)$(S_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(OBJS)$(FLASH_IMAGE)$(C_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) IoShieldLib.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
