#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel

#set -x
echo "DEFINITIONS"
./extract.sh macro SIMPLE_MACRO file_sample.h
./extract.sh macro LOGCRAZY file_sample.h
./extract.sh extern-function vAssertCalled file_sample.h
./extract.sh extern-variable MyExternVar file_sample.h
./extract.sh global LogTextOrigin file_sample.h -DLOG_ORIGIN_COUNT=
./extract.sh function myFunction file_sample.h
./extract.sh global uwTick file_sample.h -D__IO=
./extract.sh function HAL_IncTick file_sample.h -D__weak=
./extract.sh prototype myFunction file_sample.h
./extract.sh typedef myTypedef file_sample.h
./extract.sh struct MyStruct file_sample.h
./extract.sh enum MyEnum file_sample.h
./extract.sh union MyUnion file_sample.h
./extract.sh global debugClient file_sample.h
./extract.sh prototype xTaskCreate file_sample.h -DconfigSUPPORT_DYNAMIC_ALLOCATION=1 -DPRIVILEGED_FUNCTION=
./extract.sh global deviceID file_sample.h
./extract.sh function MCU_SetupHardware file_sample.h
echo "LINES"
./extract.sh macro SIMPLE_MACRO file_sample.h lines
./extract.sh macro LOGCRAZY file_sample.h lines
./extract.sh extern-function vAssertCalled file_sample.h lines
./extract.sh extern-variable MyExternVar file_sample.h lines
./extract.sh global LogTextOrigin file_sample.h lines -DLOG_ORIGIN_COUNT=
./extract.sh function myFunction file_sample.h lines
./extract.sh global uwTick file_sample.h lines -D__IO=
./extract.sh function HAL_IncTick file_sample.h lines -D__weak=
./extract.sh prototype myFunction file_sample.h lines
./extract.sh typedef myTypedef file_sample.h lines
./extract.sh struct MyStruct file_sample.h lines
./extract.sh enum MyEnum file_sample.h lines
./extract.sh union MyUnion file_sample.h lines
./extract.sh global debugClient file_sample.h lines
./extract.sh prototype xTaskCreate file_sample.h lines -DconfigSUPPORT_DYNAMIC_ALLOCATION=1 -DPRIVILEGED_FUNCTION=
./extract.sh global deviceID file_sample.h lines
./extract.sh function MCU_SetupHardware file_sample.h lines

./extract.sh function HAL_RTC_Init ../mock_tree/STM32Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c lines -DHAL_RTC_MODULE_ENABLED
./extract.sh function HAL_RTC_Init /home/moschiel/Development/EmulandoMSC/MSC_Simulator/mock_tree/STM32Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rtc.c lines -DHAL_RTC_MODULE_ENABLED