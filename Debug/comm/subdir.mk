################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../comm/server.c \
../comm/writer.c 

OBJS += \
./comm/server.o \
./comm/writer.o 

C_DEPS += \
./comm/server.d \
./comm/writer.d 


# Each subdirectory must supply rules for building sources it contributes
comm/%.o: ../comm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/aircodtest/eclipse-workspace/THOR/headers" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


