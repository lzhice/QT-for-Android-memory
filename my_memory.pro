#-------------------------------------------------
#
# Project created by QtCreator 2018-10-22T15:50:53
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += androidextras
RC_ICONS = pic.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_memory
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

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myscene.cpp \
    myitem.cpp

HEADERS += \
        mainwindow.h \
    myscene.h \
    myitem.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY =


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RESOURCES += \
    img.qrc

DISTFILES += \
    android-source/AndroidManifest.xml


contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-source
}

