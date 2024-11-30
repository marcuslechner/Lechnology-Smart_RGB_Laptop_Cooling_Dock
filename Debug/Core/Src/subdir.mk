################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/APP_DISPLAY.c \
../Core/Src/HDW_BME280.c \
../Core/Src/HDW_ENCODER.c \
../Core/Src/HDW_FAN.c \
../Core/Src/HDW_LED.c \
../Core/Src/SYS_TIMER.c \
../Core/Src/fonts.c \
../Core/Src/ftoa.c \
../Core/Src/gfx.c \
../Core/Src/main.c \
../Core/Src/oled.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/APP_DISPLAY.o \
./Core/Src/HDW_BME280.o \
./Core/Src/HDW_ENCODER.o \
./Core/Src/HDW_FAN.o \
./Core/Src/HDW_LED.o \
./Core/Src/SYS_TIMER.o \
./Core/Src/fonts.o \
./Core/Src/ftoa.o \
./Core/Src/gfx.o \
./Core/Src/main.o \
./Core/Src/oled.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/APP_DISPLAY.d \
./Core/Src/HDW_BME280.d \
./Core/Src/HDW_ENCODER.d \
./Core/Src/HDW_FAN.d \
./Core/Src/HDW_LED.d \
./Core/Src/SYS_TIMER.d \
./Core/Src/fonts.d \
./Core/Src/ftoa.d \
./Core/Src/gfx.d \
./Core/Src/main.d \
./Core/Src/oled.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/APP_DISPLAY.cyclo ./Core/Src/APP_DISPLAY.d ./Core/Src/APP_DISPLAY.o ./Core/Src/APP_DISPLAY.su ./Core/Src/HDW_BME280.cyclo ./Core/Src/HDW_BME280.d ./Core/Src/HDW_BME280.o ./Core/Src/HDW_BME280.su ./Core/Src/HDW_ENCODER.cyclo ./Core/Src/HDW_ENCODER.d ./Core/Src/HDW_ENCODER.o ./Core/Src/HDW_ENCODER.su ./Core/Src/HDW_FAN.cyclo ./Core/Src/HDW_FAN.d ./Core/Src/HDW_FAN.o ./Core/Src/HDW_FAN.su ./Core/Src/HDW_LED.cyclo ./Core/Src/HDW_LED.d ./Core/Src/HDW_LED.o ./Core/Src/HDW_LED.su ./Core/Src/SYS_TIMER.cyclo ./Core/Src/SYS_TIMER.d ./Core/Src/SYS_TIMER.o ./Core/Src/SYS_TIMER.su ./Core/Src/fonts.cyclo ./Core/Src/fonts.d ./Core/Src/fonts.o ./Core/Src/fonts.su ./Core/Src/ftoa.cyclo ./Core/Src/ftoa.d ./Core/Src/ftoa.o ./Core/Src/ftoa.su ./Core/Src/gfx.cyclo ./Core/Src/gfx.d ./Core/Src/gfx.o ./Core/Src/gfx.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/oled.cyclo ./Core/Src/oled.d ./Core/Src/oled.o ./Core/Src/oled.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

