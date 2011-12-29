################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../WInterrupts.c \
../pins_arduino.c \
../wiring.c \
../wiring_analog.c \
../wiring_digital.c \
../wiring_pulse.c \
../wiring_shift.c 

CPP_SRCS += \
../HardwareSerial.cpp \
../Print.cpp \
../Tone.cpp \
../WMath.cpp \
../WString.cpp 

CPP_DEPS += \
./HardwareSerial.d \
./Print.d \
./Tone.d \
./WMath.d \
./WString.d 

OBJS += \
./HardwareSerial.o \
./Print.o \
./Tone.o \
./WInterrupts.o \
./WMath.o \
./WString.o \
./pins_arduino.o \
./wiring.o \
./wiring_analog.o \
./wiring_digital.o \
./wiring_pulse.o \
./wiring_shift.o 

C_DEPS += \
./WInterrupts.d \
./pins_arduino.d \
./wiring.d \
./wiring_analog.d \
./wiring_digital.d \
./wiring_pulse.d \
./wiring_shift.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I/usr/local/CrossPack-AVR-20100115/avr-4/include -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/usr/local/CrossPack-AVR-20100115/avr-4/include -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


