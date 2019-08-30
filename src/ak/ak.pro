#-------------------------------------------------
#
# Project created by QtCreator 2019-08-22T08:38:38
#
#-------------------------------------------------

QT       += core gui concurrent multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ak
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

win32: {
    include($$PWD/../opencv/opencv.pri)
}


# Google breakpad
BREAKPADDIR = $$PWD/3rdparty/gbreakpad/src
DEPENDPATH += $$BREAKPADDIR

INCLUDEPATH += $$BREAKPADDIR/

win32* {
    HEADERS += $$BREAKPADDIR/common/windows/string_utils-inl.h
    HEADERS += $$BREAKPADDIR/common/windows/guid_string.h
    HEADERS += $$BREAKPADDIR/client/windows/handler/exception_handler.h
    HEADERS += $$BREAKPADDIR/client/windows/common/ipc_protocol.h
    HEADERS += $$BREAKPADDIR/google_breakpad/common/minidump_format.h
    HEADERS += $$BREAKPADDIR/google_breakpad/common/breakpad_types.h
    HEADERS += $$BREAKPADDIR/client/windows/crash_generation/crash_generation_client.h
    HEADERS += $$BREAKPADDIR/common/scoped_ptr.h
    SOURCES += $$BREAKPADDIR/client/windows/handler/exception_handler.cc
    SOURCES += $$BREAKPADDIR/common/windows/string_utils.cc
    SOURCES += $$BREAKPADDIR/common/windows/guid_string.cc
    SOURCES += $$BREAKPADDIR/client/windows/crash_generation/crash_generation_client.cc
}

SOURCES += \
        LesCrashHandler.cpp \
        adbhelper.cpp \
        alg.cpp \
        form/uidemo1.cpp \
        main.cpp \
        tesseract.cpp

HEADERS += \
        LesCrashHandler.h \
        adbhelper.h \
        alg.h \
        form/uidemo1.h \
        tesseract.h

FORMS += \
        form/uidemo1.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

PRECOMPILED_HEADER  = head.h

INCLUDEPATH     += $$PWD
CONFIG          += qt warn_off

SOURCES += main.cpp\
    form/quiwidget.cpp \
    form/appinit.cpp

HEADERS  += head.h\
    $$PWD/icon_define.h \
    form/quiwidget.h \
    form/appinit.h

RESOURCES += \
    other/qss.qrc \
    other/main.qrc \
    form/demo.qrc \
    form/image.qrc \
    sound.qrc

#release下生成pdb文件
win32:CONFIG(release, debug|release):QMAKE_LFLAGS_RELEASE += /MAP
win32:CONFIG(release, debug|release):QMAKE_CFLAGS_RELEASE += /Zi
win32:CONFIG(release, debug|release):QMAKE_LFLAGS_RELEASE += /debug /opt:ref
