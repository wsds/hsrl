# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
APP_ABI := armeabi
APP_MODULES := libhsrl-frame 

LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE    := libhsrl-frame
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := hsrl/SpaceHolder.cpp hsrl/tools.cpp prelibs/native_app_glue/android_native_app_glue.c MainEngine.cpp mainEntry.cpp mainOpenGL.cpp
LOCAL_SHARED_LIBRARIES := sooo1
LOCAL_STATIC_LIBRARIES := add
LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lEGL

include $(BUILD_SHARED_LIBRARY)

include $(LOCAL_PATH)/prebuilt/Android.mk