################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32h743zitx.s 

OBJS += \
./Startup/startup_stm32h743zitx.o 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -I../ -x assembler-with-cpp --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

