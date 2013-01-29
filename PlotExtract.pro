#-------------------------------------------------
#
# Project created by QtCreator 2011-11-17T12:49:17
#
#-------------------------------------------------

QT       += core gui

TARGET = PlotExtract
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = debug
} else {
    DESTDIR = release
}

INCLUDEPATH += ../cpp-classes/src

SOURCES +=  main.cpp\
            plotextract.cpp \
            clickablepixmap.cpp \
            credits.cpp

HEADERS  += plotextract.h \
            clickablepixmap.h \
            credits.h

FORMS    += plotextract.ui \
            credits.ui

OTHER_FILES += \
            WindowsDependencies.txt

RESOURCES += \
            app_figs.qrc


win32{
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

