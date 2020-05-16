################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/main.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/main.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32L4R5xx -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/BasicM4M3/IRQ_LL_example/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/BasicM4M3/IRQ_LL_example/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/BasicM4M3/IRQ_LL_example/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/BasicM4M3/IRQ_LL_example/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/BasicM4M3/IRQ_LL_example/Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


