################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/data.c \
../Src/it.c \
../Src/main.c \
../Src/msp.c \
../Src/syscalls.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/data.o \
./Src/it.o \
./Src/main.o \
./Src/msp.o \
./Src/syscalls.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/data.d \
./Src/it.d \
./Src/main.d \
./Src/msp.d \
./Src/syscalls.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DSTM32L4R5xx -DDEBUG -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/SLEEP_ON_EXIT_1/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/SLEEP_ON_EXIT_1/Drivers/CMSIS/Include" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/SLEEP_ON_EXIT_1/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/SLEEP_ON_EXIT_1/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/Users/khoahuynh/Documents/Document/STM32L4R5ZIT6_NucleoL4R5ZI/Eclipse/MasteringMCU2-PracticeWS/SLEEP_ON_EXIT_1/Inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

