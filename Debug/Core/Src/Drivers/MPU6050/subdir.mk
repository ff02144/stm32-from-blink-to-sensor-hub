################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Drivers/MPU6050/MPU6050.c 

OBJS += \
./Core/Src/Drivers/MPU6050/MPU6050.o 

C_DEPS += \
./Core/Src/Drivers/MPU6050/MPU6050.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Drivers/MPU6050/%.o Core/Src/Drivers/MPU6050/%.su Core/Src/Drivers/MPU6050/%.cyclo: ../Core/Src/Drivers/MPU6050/%.c Core/Src/Drivers/MPU6050/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Drivers-2f-MPU6050

clean-Core-2f-Src-2f-Drivers-2f-MPU6050:
	-$(RM) ./Core/Src/Drivers/MPU6050/MPU6050.cyclo ./Core/Src/Drivers/MPU6050/MPU6050.d ./Core/Src/Drivers/MPU6050/MPU6050.o ./Core/Src/Drivers/MPU6050/MPU6050.su

.PHONY: clean-Core-2f-Src-2f-Drivers-2f-MPU6050

