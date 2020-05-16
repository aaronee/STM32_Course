################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32l4r5xx.S 

OBJS += \
./startup/startup_stm32l4r5xx.o 

S_UPPER_DEPS += \
./startup/startup_stm32l4r5xx.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Third-Party/FreeRTOS/org/Source/include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Third-Party/Segger/Config" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Third-Party/Segger/OS" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/RTOS_workspace/FreeRTOS_LED_and_Button_IT/Third-Party/Segger/SEGGER" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


