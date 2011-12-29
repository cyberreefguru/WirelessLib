################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SoftwareSerial.cpp 

CPP_DEPS += \
./SoftwareSerial.d 

OBJS += \
./SoftwareSerial.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Users/tom/Development/arduino/avrlib" -I/usr/local/CrossPack-AVR-20100115/include -I/usr/local/CrossPack-AVR-20100115/avr-4/include -Wall -Os -fpack-struct -fshort-enums -std=gnu++98 -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


