LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := netcommom_static

LOCAL_MODULE_FILENAME := libnetcommom

LOCAL_CPPFLAGS += -DCLIENT_USE -fexceptions

LOCAL_SRC_FILES :=\
    net/crc32.cpp \
    net/daybreak_connection.cpp \
    net/eqstream.cpp \
    net/packet.cpp \
    net/servertalk_client_connection.cpp \
    net/servertalk_legacy_client_connection.cpp \
    net/tcp_connection.cpp \
    net/tcp_server.cpp \
    encryption.cpp \
    eq_packet.cpp \
    base_packet.cpp \
    opcodemgr.cpp \
    emu_opcodes.cpp \
    mutex.cpp \
    unix.cpp \
    misc.cpp \
    packet_dump.cpp \
    crc16.cpp \
    crc32.cpp \
    packet_functions.cpp \
    platform.cpp


LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/../libs/libuv/include \
$(LOCAL_PATH)/../libs/format \
$(LOCAL_PATH)/../libs/cereal


                    
#LOCAL_CFLAGS := -D__ANDROID__ -fexceptions 
#LOCAL_LDLIBS := -lGLESv2  -lEGL -llog -lz -landroid

LOCAL_WHOLE_STATIC_LIBRARIES := cocos_crypto_static
                                   
include $(BUILD_STATIC_LIBRARY)

$(call import-add-path, $(ENGINE_SRC_DIR))
$(call import-module, external/openssl/prebuilt/android)

                           
