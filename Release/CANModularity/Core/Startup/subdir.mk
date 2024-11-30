################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../CANModularity/Core/Startup/startup_stm32f103c8tx.s 

OBJS += \
./CANModularity/Core/Startup/startup_stm32f103c8tx.o 

S_DEPS += \
./CANModularity/Core/Startup/startup_stm32f103c8tx.d 


# Each subdirectory must supply rules for building sources it contributes
CANModularity/Core/Startup/%.o: ../CANModularity/Core/Startup/%.s CANModularity/Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-CANModularity-2f-Core-2f-Startup

clean-CANModularity-2f-Core-2f-Startup:
	-$(RM) ./CANModularity/Core/Startup/startup_stm32f103c8tx.d ./CANModularity/Core/Startup/startup_stm32f103c8tx.o

.PHONY: clean-CANModularity-2f-Core-2f-Startup

