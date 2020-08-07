################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ui/qr.c \
../ui/qr_generator.c 

OBJS += \
./ui/qr.o \
./ui/qr_generator.o 

C_DEPS += \
./ui/qr.d \
./ui/qr_generator.d 


# Each subdirectory must supply rules for building sources it contributes
ui/%.o: ../ui/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../headers/ -O3 -w -Wall -Wextra -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


