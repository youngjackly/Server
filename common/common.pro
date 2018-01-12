
TEMPLATE = lib

QT -= core gui
CONFIG -= qml_debug

CONFIG += staticlib
CONFIG += object_parallel_to_source
CONFIG += c++11

DEFINES += CLIENT_USE

INCLUDEPATH += $$PWD
INCLUDEPATH += ../libs/libuv/include
INCLUDEPATH += ../libs/format
INCLUDEPATH += ../libs/cereal

HEADERS += \
    $$PWD/net/console_server.h \
    $$PWD/net/console_server_connection.h \
    $$PWD/net/crc32.h \
    $$PWD/net/daybreak_connection.h \
    $$PWD/net/daybreak_structs.h \
    $$PWD/net/dns.h \
    $$PWD/net/endian.h \
    $$PWD/net/eqstream.h \
    $$PWD/net/packet.h \
    $$PWD/net/servertalk_client_connection.h \
    $$PWD/net/servertalk_common.h \
    $$PWD/net/servertalk_legacy_client_connection.h \
    $$PWD/net/servertalk_server.h \
    $$PWD/net/servertalk_server_connection.h \
    $$PWD/net/tcp_connection.h \
    $$PWD/net/tcp_server.h

SOURCES += \
    $$PWD/net/console_server.cpp \
    $$PWD/net/console_server_connection.cpp \
    $$PWD/net/crc32.cpp \
    $$PWD/net/daybreak_connection.cpp \
    $$PWD/net/eqstream.cpp \
    $$PWD/net/packet.cpp \
    $$PWD/net/servertalk_client_connection.cpp \
    $$PWD/net/servertalk_legacy_client_connection.cpp \
    $$PWD/net/servertalk_server.cpp \
    $$PWD/net/servertalk_server_connection.cpp \
    $$PWD/net/tcp_connection.cpp \
    $$PWD/net/tcp_server.cpp \
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
