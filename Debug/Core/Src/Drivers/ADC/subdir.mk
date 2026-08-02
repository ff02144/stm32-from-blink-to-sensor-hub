################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Drivers/ADC/ADC.c 

OBJS += \
./Core/Src/Drivers/ADC/ADC.o 

C_DEPS += \
./Core/Src/Drivers/ADC/ADC.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Drivers/ADC/%.o Core/Src/Drivers/ADC/%.su Core/Src/Drivers/ADC/%.cyclo: ../Core/Src/Drivers/ADC/%.c Core/Src/Drivers/ADC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Drivers-2f-ADC

clean-Core-2f-Src-2f-Drivers-2f-ADC:
	-$(RM) ./Core/Src/Drivers/ADC/ADC.cyclo ./Core/Src/Drivers/ADC/ADC.d ./Core/Src/Drivers/ADC/ADC.o ./Core/Src/Drivers/ADC/ADC.su

.PHONY: clean-Core-2f-Src-2f-Drivers-2f-ADC

