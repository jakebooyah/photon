LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/GameScene.cpp \
                   ../../Classes/NetworkRoomScene.cpp \
                   ../../Classes/CreateRoomScene.cpp \
                   ../../Classes/JoinRoomScene.cpp \
                   ../../Classes/NetworkEngine.cpp \
                   ../../Classes/TwoPlayerGameScene.cpp \
                   ../../Classes/FourPlayerGameScene.cpp \
                   ../../Classes/MainMenuScene.cpp \
                   ../../Classes/GameOverScene.cpp \
                   ../../Classes/NetworkLogic.cpp \
                   ../../Classes/ContactListener.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../Photon-AndroidNDK_SDK
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../external/Box2D
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../extensions/GUI/CCEditBox

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

LOCAL_CFLAGS := -DEG_DEBUGGER -D__STDINT_LIMITS -D_EG_ANDROID_PLATFORM
LOCAL_STATIC_LIBRARIES := loadbalancing-cpp-static-prebuilt photon-cpp-static-prebuilt common-cpp-static-prebuilt
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)

$(call import-add-path,$(shell pwd)/Photon-AndroidNDK_SDK/Common-cpp)
$(call import-add-path,$(shell pwd)/Photon-AndroidNDK_SDK/Photon-cpp)
$(call import-add-path,$(shell pwd)/Photon-AndroidNDK_SDK/LoadBalancing-cpp)
$(call import-add-path,$(shell pwd)/Photon-AndroidNDK_SDK/Chat-cpp)

$(call import-module,loadbalancing-cpp-prebuilt)
