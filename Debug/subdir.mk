################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../astrolib.c \
../main.c \
../reductiontest1.c \
../reductiontest2.c \
../vsop87a_full.c \
../vsop87a_full_velocities.c 

OBJS += \
./astrolib.o \
./main.o \
./reductiontest1.o \
./reductiontest2.o \
./vsop87a_full.o \
./vsop87a_full_velocities.o 

C_DEPS += \
./astrolib.d \
./main.d \
./reductiontest1.d \
./reductiontest2.d \
./vsop87a_full.d \
./vsop87a_full_velocities.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"C:\prog\Astronomy\sofaIAU\sofa\20180130\c\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


