################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FinalProject_MC2.c \
../external_eeprom.c \
../i2c.c \
../timer1.c \
../uart.c 

OBJS += \
./FinalProject_MC2.o \
./external_eeprom.o \
./i2c.o \
./timer1.o \
./uart.o 

C_DEPS += \
./FinalProject_MC2.d \
./external_eeprom.d \
./i2c.d \
./timer1.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


