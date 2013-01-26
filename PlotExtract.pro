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

INCLUDEPATH += /home/rgrandin/qt-workspace/cppclasses

SOURCES += main.cpp\
        plotextract.cpp \
    clickablepixmap.cpp

HEADERS  += plotextract.h \
    clickablepixmap.h

FORMS    += plotextract.ui

OTHER_FILES += \
    WindowsDependencies.txt

RESOURCES += \
    app_figs.qrc

