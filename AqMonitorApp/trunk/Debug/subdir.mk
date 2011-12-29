################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

CPP_SRCS += \
../Lcd.cpp \
../PhStamp.cpp \
../RealTimeClock.cpp \
../TemperatureProbe.cpp 

CPP_DEPS += \
./Lcd.d \
./PhStamp.d \
./RealTimeClock.d \
./TemperatureProbe.d 

OBJS += \
./Lcd.o \
./PhStamp.o \
./RealTimeClock.o \
./TemperatureProbe.o \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Users/tom/Development/git/Arduino/Wire" -I"/Users/tom/Development/git/Arduino/SoftwareSerial" -I"/Users/tom/Development/git/Arduino/avrlib" -I/usr/local/CrossPack-AVR-20100115/avr-4/include -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-g++ -I"/Users/tom/Development/git/Arduino/Wire" -I"/Users/tom/Development/git/Arduino/SoftwareSerial" -I"/Users/tom/Development/git/Arduino/avrlib" -I/usr/local/CrossPack-AVR-20100115/avr-4/include -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


