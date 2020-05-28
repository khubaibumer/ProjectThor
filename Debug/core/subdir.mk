################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../core/thor.c 

OBJS += \
./core/thor.o 

C_DEPS += \
./core/thor.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/aircodtest/eclipse-workspace/THOR/headers" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


