LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := libuv_static

LOCAL_MODULE_FILENAME := liblibuv

LOCAL_SRC_FILES :=\
    format/fmt/test.cc \
    libuv/src/fs-poll.c \
    libuv/src/inet.c \
    libuv/src/threadpool.c \
    libuv/src/uv-common.c \
    libuv/src/version.c \
    libuv/src/unix/android-ifaddrs.c \
    libuv/src/unix/async.c \
    libuv/src/unix/core.c \
    libuv/src/unix/fs.c \
    libuv/src/unix/getaddrinfo.c \
    libuv/src/unix/getnameinfo.c \
    libuv/src/unix/linux-core.c \
    libuv/src/unix/linux-inotify.c \
    libuv/src/unix/linux-syscalls.c \
    libuv/src/unix/loop.c \
    libuv/src/unix/loop-watcher.c \
    libuv/src/unix/pipe.c \
    libuv/src/unix/poll.c \
    libuv/src/unix/process.c \
    libuv/src/unix/proctitle.c \
    libuv/src/unix/pthread-fixes.c \
    libuv/src/unix/pthread-barrier.c \
    libuv/src/unix/signal.c \
    libuv/src/unix/stream.c \
    libuv/src/unix/tcp.c \
    libuv/src/unix/thread.c \
    libuv/src/unix/timer.c \
    libuv/src/unix/tty.c \
    libuv/src/unix/udp.c


LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/libuv/include \
$(LOCAL_PATH)/libuv/src


                    
#LOCAL_CFLAGS := -D__ANDROID__ -fexceptions 
#LOCAL_LDLIBS := -lGLESv2  -lEGL -llog -lz -landroid
#LOCAL_WHOLE_STATIC_LIBRARIES := cocos_crypto_static
                                   
include $(BUILD_STATIC_LIBRARY)


                           
