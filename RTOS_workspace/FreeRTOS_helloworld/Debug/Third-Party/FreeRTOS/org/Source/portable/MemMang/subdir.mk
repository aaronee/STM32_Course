################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.c 

OBJS += \
./Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/FreeRTOS/org/Source/portable/MemMang/%.o: ../Third-Party/FreeRTOS/org/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_FULL_LL_DRIVER -D__NVIC_PRIO_BITS=4 -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Third-Party/FreeRTOS/org/Source/include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_helloworld/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -std=gnu11 -c -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=gnu90 -O0 -ffunction-sections -fdata-sections -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


