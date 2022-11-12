################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SPI_TFT_ILI9341/GraphicsDisplay.cpp \
../SPI_TFT_ILI9341/SPI_TFT_ILI9341.cpp \
../SPI_TFT_ILI9341/SPI_TFT_ILI9341_NUCLEO.cpp \
../SPI_TFT_ILI9341/SPI_TFT_ILI9341_NXP.cpp \
../SPI_TFT_ILI9341/TextDisplay.cpp 

OBJS += \
./SPI_TFT_ILI9341/GraphicsDisplay.o \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341.o \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341_NUCLEO.o \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341_NXP.o \
./SPI_TFT_ILI9341/TextDisplay.o 

CPP_DEPS += \
./SPI_TFT_ILI9341/GraphicsDisplay.d \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341.d \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341_NUCLEO.d \
./SPI_TFT_ILI9341/SPI_TFT_ILI9341_NXP.d \
./SPI_TFT_ILI9341/TextDisplay.d 


# Each subdirectory must supply rules for building sources it contributes
SPI_TFT_ILI9341/%.o: ../SPI_TFT_ILI9341/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -D__CODE_RED -DCPP_USE_HEAP -DTARGET_LPC1768 -DTARGET_M3 -DTARGET_NXP -DTARGET_LPC176X -DTARGET_MBED_LPC1768 -DTOOLCHAIN_GCC_CR -DTOOLCHAIN_GCC -D__CORTEX_M3 -DARM_MATH_CM3 -DMBED_BUILD_TIMESTAMP=1413804419.35 -D__MBED__=1 -I"C:\Users\chatchai\Desktop\my_project\app_lcd" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\TFT_fonts" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\SPI_TFT_ILI9341" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X" -I"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X\TARGET_MBED_LPC1768" -Og -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


