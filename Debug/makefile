################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include mbed/TARGET_LPC1768/TOOLCHAIN_GCC_CR/subdir.mk
-include SPI_TFT_ILI9341/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: app_lcd.axf

# Tool invocations
app_lcd.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\chatchai\Desktop\my_project\app_lcd" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\TFT_fonts" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\SPI_TFT_ILI9341" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X" -L"C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TARGET_NXP\TARGET_LPC176X\TARGET_MBED_LPC1768" -Xlinker -Map="app_lcd.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "C:\Users\chatchai\Desktop\my_project\app_lcd\mbed\TARGET_LPC1768\TOOLCHAIN_GCC_CR\LPC1768.ld" -o "app_lcd.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) app_lcd.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "app_lcd.axf"; arm-none-eabi-objcopy -O binary "app_lcd.axf" "app_lcd.bin" ; # checksum -p LPC1769 -d "app_lcd.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
