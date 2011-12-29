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
libDiecimilaCore.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: libDiecimilaCore.a secondary-outputs

# Tool invocations
libDiecimilaCore.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR Archiver'
	avr-ar -r "libDiecimilaCore.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

libDiecimilaCore.lss: libDiecimilaCore.a
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S libDiecimilaCore.a  >"libDiecimilaCore.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libDiecimilaCore.a
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p libDiecimilaCore.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_DEPS)$(ARCHIVES)$(S_UPPER_DEPS)$(C++_DEPS)$(LSS)$(S_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(OBJS)$(C_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) libDiecimilaCore.a
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
