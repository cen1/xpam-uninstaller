#-------------------------------------------------
#
# Project created by QtCreator 2014-01-04T18:06:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
DEFINES += _ATL_XP_TARGETING
TARGET = uninstaller
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    registry.cpp

HEADERS  += mainwindow.h \
    winutils.h \
    registry.h

FORMS    += mainwindow.ui

RESOURCES += \
    uninstaller.qrc

win32:CONFIG(release, debug|release): QMAKE_POST_LINK += cd $$PWD/release && copy /Y \"uninstaller.exe\" \"$$PWD/../../installer/installer/data/uninstaller.exe\" && upx \"$$PWD/../../installer/installer/data/uninstaller.exe\"

win32 {
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}
