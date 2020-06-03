################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../security/authorize.c \
../security/hash.c \
../security/ssl_tls.c 

OBJS += \
./security/authorize.o \
./security/hash.o \
./security/ssl_tls.o 

C_DEPS += \
./security/authorize.d \
./security/hash.d \
./security/ssl_tls.d 


# Each subdirectory must supply rules for building sources it contributes
security/%.o: ../security/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/aircodtest/eclipse-workspace/THOR/headers" -O0 -g3 -w -Wall -Wextra -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


