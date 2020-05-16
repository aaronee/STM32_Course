################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/it.c \
../Src/main.c \
../Src/msp.c \
../Src/stm32l4xx_hal_timebase_tim.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/it.o \
./Src/main.o \
./Src/msp.o \
./Src/stm32l4xx_hal_timebase_tim.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/it.d \
./Src/main.d \
./Src/msp.d \
./Src/stm32l4xx_hal_timebase_tim.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_FULL_LL_DRIVER -D__NVIC_PRIO_BITS=4 -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Third-Party/FreeRTOS/org/Source/include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Third-Party/Segger/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Third-Party/Segger/OS" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_Tasks_Priority/Third-Party/Segger/SEGGER" -std=gnu11 -c -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=gnu90 -O0 -ffunction-sections -fdata-sections -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


