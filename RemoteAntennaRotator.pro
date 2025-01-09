#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T14:21:20
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

TARGET = RemoteAntennaRotator
TEMPLATE = app

CONFIG(debug, debug|release) {
  BUILD_TYPE = debug
  OUT_DIR = distr/debug
}

CONFIG(release, debug|release) {
  BUILD_TYPE = release
  OUT_DIR = distr/release
}

DESTDIR = $${OUT_DIR}

SOURCES += \
    src/DataExchange.cpp \
    src/Encoder.cpp \
    src/MainWindow.cpp \
    src/Servo.cpp \
    src/ServoSettingsView.cpp \
    src/TcpSettingsView.cpp \
    src/main.cpp \
    src/Log/Log.cpp \
    src/MVP/Model.cpp \
    src/MVP/Presentor.cpp \
    src/MVP/AzElView.cpp \
    src/ProtocolUtil.cpp \
    src/Settings.cpp \
    src/MessageBus.cpp \
    src/TcpUartModule.cpp \
    src/Rotator.cpp

HEADERS  += \
    src/DataExchange.h \
    src/Encoder.h \
    src/IDataExchange.h \
    src/MainWindow.h \
    src/Servo.h \
    src/ServoSettingsView.h \
    src/TcpSettingsView.h \
    src/sleepthread.h \
    src/Log/Log.h \
    src/MVP/Model.h \
    src/MVP/Presentor.h \
    src/MVP/AzElView.h \
    src/ProtocolUtil.h \
    src/Settings.h \
    src/MessageBus.h \
    src/TcpUartModule.h \
    src/CommonConst.h \
    src/Rotator.h

FORMS    +=

DISTFILES += \
    Config/settings.ini
