#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T20:57:44
#
#-------------------------------------------------

QT       += core gui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-private-field

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-iotivity-client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CLIENTARCH = x86_64
BUILDTYPE = release

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

IOTIVITY_ROOT = /Users/dbrody/Projects/iotivity/iotivity
IOTIVITY_DYLD_PATH = $$IOTIVITY_ROOT/out/darwin/$$CLIENTARCH/$$BUILDTYPE

LIBS += -L$$IOTIVITY_DYLD_PATH \
    -lc_common \
    -lcjson \
    -lcoap \
    -lconnectivity_abstraction \
    -lconnectivity_abstraction_internal \
    -llogger \
    -lmbedcrypto \
    -lmbedtls \
    -lmbedx509 \
    -loc \
    -loc_logger_internal \
    -locsrm \
    -loctbstack \
    -loctbstack_internal \
    -lresource_directory \
    -lroutingmanager

INCLUDEPATH += \
    $$IOTIVITY_DYLD_PATH/include/c_common \
    $$IOTIVITY_ROOT/resource/include \
    $$IOTIVITY_ROOT/resource/c_common/include \
    $$IOTIVITY_ROOT/resource/csdk/include \
    $$IOTIVITY_ROOT/resource/csdk/stack/include \
    $$IOTIVITY_ROOT/resource/csdk/logger/include \
    $$IOTIVITY_ROOT/resource/csdk/connectivity/inc \
    $$IOTIVITY_ROOT/resource/csdk/connectivity/api \
    $$IOTIVITY_ROOT/resource/csdk/connectivity/common/inc \
    $$$IOTIVITY_ROOT/resource/csd   k/security/include \
    $$IOTIVITY_ROOT/resource/c_common/ocrandom/include \
    $$IOTIVITY_ROOT/resource/oc_logger/include \
    $$IOTIVITY_ROOT/c_common/oic_string/include/oic_string.h \
    $$IOTIVITY_ROOT/c_common/oic_malloc/include/oic_malloc.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    iotclient.cpp

HEADERS += \
        mainwindow.h \
    namedefs.h \
    iotclient.h

FORMS += \
        mainwindow.ui

# =========================
# For Boost
# -----------
# Make sure you have done `brew install boost`
# Careful with libs JPEG, etc
# See: https://github.com/shadowsocks/shadowsocks-qt5/issues/457

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib
LIBS += -lboost_system-mt -lboost_filesystem-mt
