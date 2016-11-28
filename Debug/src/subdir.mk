################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ComPort.cpp \
../src/NRF905Gate.cpp \
../src/STM32nFR24.cpp \
../src/Scenario.cpp \
../src/ThingSpeakClient.cpp \
../src/UDPReciver.cpp \
../src/btest.cpp \
../src/gpio.cpp \
../src/scanner.cpp \
../src/spi.cpp 

C_SRCS += \
../src/compatibility.c 

OBJS += \
./src/ComPort.o \
./src/NRF905Gate.o \
./src/STM32nFR24.o \
./src/Scenario.o \
./src/ThingSpeakClient.o \
./src/UDPReciver.o \
./src/btest.o \
./src/compatibility.o \
./src/gpio.o \
./src/scanner.o \
./src/spi.o 

C_DEPS += \
./src/compatibility.d 

CPP_DEPS += \
./src/ComPort.d \
./src/NRF905Gate.d \
./src/STM32nFR24.d \
./src/Scenario.d \
./src/ThingSpeakClient.d \
./src/UDPReciver.d \
./src/btest.d \
./src/gpio.d \
./src/scanner.d \
./src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabi-g++ -I/usr/arm-linux-gnueabi/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabi-gcc -I/usr/arm-linux-gnueabi/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


