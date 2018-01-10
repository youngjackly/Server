TEMPLATE = lib

QT -= core gui
CONFIG -= qml_debug

CONFIG += staticlib
CONFIG += object_parallel_to_source
CONFIG += c++11

QMAKE_CXXFLAGS += -fexceptions

INCLUDEPATH += libuv/src
INCLUDEPATH += libuv/include

HEADERS += \
format/fmt/format.h

SOURCES += \
    format/fmt/test.cc \
    libuv/src/fs-poll.c \
    libuv/src/inet.c \
    libuv/src/threadpool.c \
    libuv/src/uv-common.c \
    libuv/src/version.c \
    libuv/src/unix/async.c \
    libuv/src/unix/core.c \
    libuv/src/unix/fs.c \
    libuv/src/unix/getaddrinfo.c \
    libuv/src/unix/getnameinfo.c \
    libuv/src/unix/linux-core.c \
    libuv/src/unix/linux-inotify.c \
    libuv/src/unix/linux-syscalls.c \
    libuv/src/unix/loop.c \
    libuv/src/unix/pipe.c \
    libuv/src/unix/poll.c \
    libuv/src/unix/process.c \
    libuv/src/unix/proctitle.c \
    libuv/src/unix/pthread-fixes.c \
    libuv/src/unix/signal.c \
    libuv/src/unix/stream.c \
    libuv/src/unix/tcp.c \
    libuv/src/unix/thread.c \
    libuv/src/unix/timer.c \
    libuv/src/unix/tty.c \
    libuv/src/unix/udp.c


