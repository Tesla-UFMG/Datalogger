################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application.c \
../Core/Src/CAN_handler.c \
../Core/Src/Initializers.c \
../Core/Src/SD.c \
../Core/Src/SD_DMA.c \
../Core/Src/can_log.c \
../Core/Src/general_can.c \
../Core/Src/main.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c \
../Core/Src/timer_handler.c 

OBJS += \
./Core/Src/Application.o \
./Core/Src/CAN_handler.o \
./Core/Src/Initializers.o \
./Core/Src/SD.o \
./Core/Src/SD_DMA.o \
./Core/Src/can_log.o \
./Core/Src/general_can.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o \
./Core/Src/timer_handler.o 

C_DEPS += \
./Core/Src/Application.d \
./Core/Src/CAN_handler.d \
./Core/Src/Initializers.d \
./Core/Src/SD.d \
./Core/Src/SD_DMA.d \
./Core/Src/can_log.d \
./Core/Src/general_can.d \
./Core/Src/main.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d \
./Core/Src/timer_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Application.d ./Core/Src/Application.o ./Core/Src/Application.su ./Core/Src/CAN_handler.d ./Core/Src/CAN_handler.o ./Core/Src/CAN_handler.su ./Core/Src/Initializers.d ./Core/Src/Initializers.o ./Core/Src/Initializers.su ./Core/Src/SD.d ./Core/Src/SD.o ./Core/Src/SD.su ./Core/Src/SD_DMA.d ./Core/Src/SD_DMA.o ./Core/Src/SD_DMA.su ./Core/Src/can_log.d ./Core/Src/can_log.o ./Core/Src/can_log.su ./Core/Src/general_can.d ./Core/Src/general_can.o ./Core/Src/general_can.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32h7xx_hal_msp.d ./Core/Src/stm32h7xx_hal_msp.o ./Core/Src/stm32h7xx_hal_msp.su ./Core/Src/stm32h7xx_it.d ./Core/Src/stm32h7xx_it.o ./Core/Src/stm32h7xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32h7xx.d ./Core/Src/system_stm32h7xx.o ./Core/Src/system_stm32h7xx.su ./Core/Src/timer_handler.d ./Core/Src/timer_handler.o ./Core/Src/timer_handler.su

.PHONY: clean-Core-2f-Src

