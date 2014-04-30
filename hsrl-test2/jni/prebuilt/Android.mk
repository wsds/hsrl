LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS) 
LOCAL_MODULE    := add
LOCAL_SRC_FILES := libadd.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS) 
LOCAL_MODULE    := sooo1
LOCAL_SRC_FILES := libsooo1.so
include $(PREBUILT_SHARED_LIBRARY)

