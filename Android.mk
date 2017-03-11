INNER_SAVED_LOCAL_PATH := $(LOCAL_PATH)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := simp

LOCAL_C_INCLUDES  := \
	${SIMP_SRC_PATH}/include \
	${SIMP_SRC_PATH}/include/simp \
	${BIMP_SRC_PATH}/include \
	${CU_SRC_PATH} \
	${LOGGER_SRC_PATH} \
	${SM_SRC_PATH} \
	${PS_SRC_PATH} \
	${FS_SRC_PATH} \
	${MTRAIL_SRC_PATH} \


LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,,$(shell find $(LOCAL_PATH) -name "*.cpp" -print)) \

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(INNER_SAVED_LOCAL_PATH)