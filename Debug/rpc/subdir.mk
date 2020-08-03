################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rpc/clight.c \
../rpc/db_manager.c \
../rpc/interface.c 

OBJS += \
./rpc/clight.o \
./rpc/db_manager.o \
./rpc/interface.o 

C_DEPS += \
./rpc/clight.d \
./rpc/db_manager.d \
./rpc/interface.d 


# Each subdirectory must supply rules for building sources it contributes
rpc/%.o: ../rpc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../headers/ -O0 -g3 -w -Wall -Wextra -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


