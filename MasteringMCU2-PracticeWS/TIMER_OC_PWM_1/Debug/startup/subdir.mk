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
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/TIMER_OC_PWM_1/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/TIMER_OC_PWM_1/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/TIMER_OC_PWM_1/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/TIMER_OC_PWM_1/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/TIMER_OC_PWM_1/Inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


