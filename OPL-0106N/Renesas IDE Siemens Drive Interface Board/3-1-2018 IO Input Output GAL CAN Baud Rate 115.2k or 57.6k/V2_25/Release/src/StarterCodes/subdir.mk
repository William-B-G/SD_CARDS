################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/StarterCodes/dbsct.c \
..\src/StarterCodes/interrupt_handlers.c \
..\src/StarterCodes/reset_program.c \
..\src/StarterCodes/sbrk.c \
..\src/StarterCodes/vector_table.c 

C_DEPS += \
./src/StarterCodes/dbsct.d \
./src/StarterCodes/interrupt_handlers.d \
./src/StarterCodes/reset_program.d \
./src/StarterCodes/sbrk.d \
./src/StarterCodes/vector_table.d 

OBJS += \
./src/StarterCodes/dbsct.obj \
./src/StarterCodes/interrupt_handlers.obj \
./src/StarterCodes/reset_program.obj \
./src/StarterCodes/sbrk.obj \
./src/StarterCodes/vector_table.obj 


# Each subdirectory must supply rules for building sources it contributes
src/StarterCodes/%.obj: ../src/StarterCodes/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\PROGRA~2\Renesas\RX\2_1_0/include" -D__RX   -D__RX600=1  -D__LIT=1 -D__RON=1 -D__UCHAR=1 -D__DBL4=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199409L -U__STDC_HOSTED__  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02010000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c -output=obj="$(@:%.d=%.obj)"  -include="C:\PROGRA~2\Renesas\RX\2_1_0/include"  -nomessage -cpu=rx600 -alias=noansi -nologo -change_message=warning  -define=__RX   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

