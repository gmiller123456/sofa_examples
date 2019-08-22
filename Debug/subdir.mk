################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c \
../reductiontest1.c \
../vsop87a_full.c 

OBJS += \
./main.o \
./reductiontest1.o \
./vsop87a_full.o 

C_DEPS += \
./main.d \
./reductiontest1.d \
./vsop87a_full.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"C:\prog\Astronomy\sofaIAU\sofa\20180130\c\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


