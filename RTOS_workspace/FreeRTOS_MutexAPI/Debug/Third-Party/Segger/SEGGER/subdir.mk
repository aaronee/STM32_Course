################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/Segger/SEGGER/SEGGER_RTT.c \
../Third-Party/Segger/SEGGER/SEGGER_RTT_printf.c \
../Third-Party/Segger/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
../Third-Party/Segger/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./Third-Party/Segger/SEGGER/SEGGER_RTT.o \
./Third-Party/Segger/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./Third-Party/Segger/SEGGER/SEGGER_RTT_printf.o \
./Third-Party/Segger/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./Third-Party/Segger/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./Third-Party/Segger/SEGGER/SEGGER_RTT.d \
./Third-Party/Segger/SEGGER/SEGGER_RTT_printf.d \
./Third-Party/Segger/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/Segger/SEGGER/%.o: ../Third-Party/Segger/SEGGER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_FULL_LL_DRIVER -D__NVIC_PRIO_BITS=4 -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/FreeRTOS/org/Source/include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/OS" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/SEGGER" -std=gnu11 -c -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -std=gnu90 -O0 -ffunction-sections -fdata-sections -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Third-Party/Segger/SEGGER/%.o: ../Third-Party/Segger/SEGGER/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/FreeRTOS/org/Source/include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/OS" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_MutexAPI/Third-Party/Segger/SEGGER" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


