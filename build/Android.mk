# ---------------------------------------------------------------------------------
# 			Make the library (libmmrtip)
# ---------------------------------------------------------------------------------

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

commonIncludes := $(QC_PROP_ROOT)/gles/adreno200/include/public \
				  $(LOCAL_PATH)/../inc/ \

commonCflags   := -c -g -O0  -W -x c++ \
	-DANDROID \
	-DQUALCOMM_NO_EXCEPTIONS \
	-DSTDC_LIMIT_MACROS \
	-DPLATFORM_POSIX \
	-DSTDC_CONSTANT_MACROS

#Name of the target to build
LOCAL_MODULE 			:= libmmrtip

#Include
LOCAL_C_INCLUDES 		+= $(commonIncludes)

#Sources
LOCAL_SRC_FILES			:= ../src/ImProc.cpp \
					+= ../src/ImProcUtilColorTArm.c \
					+= ../src/ImProcGLUtil.cpp \
					+= ../src/ImProcCL_YUVNormal.cpp \
					+= ../src/CL_BufCopy.cpp \

LOCAL_PRELINK_MODULE 	:= false

LOCAL_SHARED_LIBRARIES	:= libcutils liblog libGLESv2 libEGL libOpenCL libdl
LOCAL_MODULE_TAGS		:= optional

LOCAL_CFLAGS    		+= $(commonCflags)

include $(BUILD_SHARED_LIBRARY)

# ---------------------------------------------------------------------------------
# 			Make the unit test
# ---------------------------------------------------------------------------------

include $(CLEAR_VARS)

#Name of the target to build
LOCAL_MODULE			:= mm-rtip-test

LOCAL_PRELINK_MODULE	:= true
LOCAL_SHARED_LIBRARIES	:= libcutils libmmrtip liblog libutils
LOCAL_MODULE_TAGS		:= optional

#Include
LOCAL_C_INCLUDES 		:= $(LOCAL_PATH)/../inc/

#Sources
LOCAL_SRC_FILES			:= ../test/rtip_test.cpp

#Make it executable
include $(BUILD_EXECUTABLE)

