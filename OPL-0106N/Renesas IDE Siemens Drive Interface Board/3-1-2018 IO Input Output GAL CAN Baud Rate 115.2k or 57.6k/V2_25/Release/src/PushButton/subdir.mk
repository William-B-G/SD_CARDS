################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/PushButton/PushButton.c 

OBJS += \
./src/PushButton/PushButton.obj 

C_DEPS += \
./src/PushButton/PushButton.d 


# Each subdirectory must supply rules for building sources it contributes
src/PushButton/%.obj: ../src/PushButton/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\PROGRA~2\Renesas\RX\2_0_1/include" -D__RX   -D__RX600=1  -D__LIT=1 -D__RON=1 -D__UCHAR=1 -D__DBL4=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D_STDC_VERSION_=199409L -U_STDC_HOSTED_  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000100 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c -output=obj="$(@:%.d=%.obj)"  -include="C:\PROGRA~2\Renesas\RX\2_0_1/include"  -cpu=rx600 -nologo -change_message=warning  -define=__RX   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

