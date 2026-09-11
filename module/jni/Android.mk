LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := imgui
LOCAL_SRC_FILES := ../../include/imgui/imgui.cpp \
                   ../../include/imgui/imgui_draw.cpp \
                   ../../include/imgui/imgui_tables.cpp \
                   ../../include/imgui/imgui_widgets.cpp \
                   ../../include/imgui/backends/imgui_impl_opengl3.cpp \
                   ../../include/imgui/backends/imgui_impl_android.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include/imgui \
                    $(LOCAL_PATH)/../../include/imgui/backends
LOCAL_CPPFLAGS := -std=c++17 -fno-exceptions -fno-rtti -DIMGUI_IMPL_OPENGL_ES3
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ffmax_injector
LOCAL_SRC_FILES := ../../src/main.cpp \
                   ../../src/memory/Memory.cpp \
                   ../../src/memory/Il2Cpp.cpp \
                   ../../src/hacks/ESP.cpp \
                   ../../src/hacks/Aimbot.cpp \
                   ../../src/hacks/EntityUpdater.cpp \
                   ../../src/hacks/Menu.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include \
                    $(LOCAL_PATH)/../../include/imgui \
                    $(LOCAL_PATH)/../../include/imgui/backends \
                    $(LOCAL_PATH)/../../src \
                    $(LOCAL_PATH)/../../src/hacks \
                    $(LOCAL_PATH)/../../src/memory
LOCAL_CPPFLAGS := -std=c++17 -fno-exceptions -fno-rtti -fvisibility=hidden
LOCAL_LDLIBS := -llog -lEGL -lGLESv3 -landroid -ldl
LOCAL_STATIC_LIBRARIES := imgui
include $(BUILD_SHARED_LIBRARY)
