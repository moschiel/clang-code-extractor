#!/bin/bash
#  Created on: 21 de jan de 2025
#      Author: roger.moschiel

set -x

./extract.sh macro SIMPLE_MACRO file_sample.h
./extract.sh macro LOGCRAZY file_sample.h
./extract.sh extern-function vAssertCalled file_sample.h
./extract.sh extern-variable MyExternVar file_sample.h
./extract.sh global LogTextOrigin file_sample.h "-DLOG_ORIGIN_COUNT="
./extract.sh function myFunction file_sample.h
./extract.sh global uwTick file_sample.h "-D__IO=volatile"
./extract.sh function HAL_IncTick file_sample.h "-D__weak=__attribute__((weak))"
./extract.sh prototype myFunction file_sample.h
./extract.sh typedef myTypedef file_sample.h
./extract.sh struct MyStruct file_sample.h
./extract.sh enum MyEnum file_sample.h
./extract.sh union MyUnion file_sample.h
./extract.sh global debugClient file_sample.h
./extract.sh prototype xTaskCreate file_sample.h "-DconfigSUPPORT_DYNAMIC_ALLOCATION=1 -DPRIVILEGED_FUNCTION="
./extract.sh global deviceID file_sample.h