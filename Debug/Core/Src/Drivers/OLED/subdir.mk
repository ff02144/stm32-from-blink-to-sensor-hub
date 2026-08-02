################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Drivers/OLED/OLED.c 

OBJS += \
./Core/Src/Drivers/OLED/OLED.o 

C_DEPS += \
./Core/Src/Drivers/OLED/OLED.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Drivers/OLED/%.o Core/Src/Drivers/OLED/%.su Core/Src/Drivers/OLED/%.cyclo: ../Core/Src/Drivers/OLED/%.c Core/Src/Drivers/OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Drivers-2f-OLED

clean-Core-2f-Src-2f-Drivers-2f-OLED:
	-$(RM) ./Core/Src/Drivers/OLED/OLED.cyclo ./Core/Src/Drivers/OLED/OLED.d ./Core/Src/Drivers/OLED/OLED.o ./Core/Src/Drivers/OLED/OLED.su

.PHONY: clean-Core-2f-Src-2f-Drivers-2f-OLED

