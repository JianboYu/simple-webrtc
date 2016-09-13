LOCAL_PATH := $(call my-dir)

ifneq (,$(filter arm arm64, $(TARGET_ARCH)))
 include $(LOCAL_PATH)/opus.target.linux-arm.mk
else
 include $(LOCAL_PATH)/opus.target.linux-x86.mk
endif
