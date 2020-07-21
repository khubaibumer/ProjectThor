################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../database/items_images.c \
../database/items_info_db.c \
../database/rpc_log_db.c \
../database/thor_db.c \
../database/usr_info_db.c 

OBJS += \
./database/items_images.o \
./database/items_info_db.o \
./database/rpc_log_db.o \
./database/thor_db.o \
./database/usr_info_db.o 

C_DEPS += \
./database/items_images.d \
./database/items_info_db.d \
./database/rpc_log_db.d \
./database/thor_db.d \
./database/usr_info_db.d 


# Each subdirectory must supply rules for building sources it contributes
database/%.o: ../database/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I../headers/ -O3 -w -Wall -Wextra -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


