################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../buzzer.c \
../dc_motor.c \
../flame_sensor.c \
../gpio.c \
../lcd.c \
../ldr_sensor.c \
../led.c \
../lm35_sensor.c \
../pwm_timer0.c \
../smart_home.c 

OBJS += \
./adc.o \
./buzzer.o \
./dc_motor.o \
./flame_sensor.o \
./gpio.o \
./lcd.o \
./ldr_sensor.o \
./led.o \
./lm35_sensor.o \
./pwm_timer0.o \
./smart_home.o 

C_DEPS += \
./adc.d \
./buzzer.d \
./dc_motor.d \
./flame_sensor.d \
./gpio.d \
./lcd.d \
./ldr_sensor.d \
./led.d \
./lm35_sensor.d \
./pwm_timer0.d \
./smart_home.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


