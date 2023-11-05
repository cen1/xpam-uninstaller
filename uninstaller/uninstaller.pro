#-------------------------------------------------
#
# Project created by QtCreator 2014-01-04T18:06:05
#
#-------------------------------------------------

QT += core gui widgets

#QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
#DEFINES += _ATL_XP_TARGETING

TARGET = uninstaller
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    registry.cpp

HEADERS  += mainwindow.h \
    winutils.h \
    registry.h

FORMS += mainwindow.ui

RESOURCES += \
    uninstaller.qrc

win32:CONFIG(release, debug|release): QMAKE_POST_LINK += cd $$PWD/../build-uninstaller-qt6_static-Release/release && copy /Y \"uninstaller.exe\" \"$$PWD/../../xpam-installer/installer/data/uninstaller.exe\" && upx \"$$PWD/../../xpam-installer/installer/data/uninstaller.exe\"

win32 {
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}
