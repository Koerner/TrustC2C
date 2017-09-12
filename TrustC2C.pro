#-------------------------------------------------
#
# Project created by QtCreator 2017-08-10T15:21:49
#
#-------------------------------------------------

QT += core gui
QT += charts
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CXXFLAGS+=-gdwarf-2

TARGET = TrustC2C
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#deactivate debug in release
CONFIG(release, debug|release) {
    #This is a release build
    DEFINES += QT_NO_DEBUG_OUTPUT
} else {
    #This is a debug build
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    interaction.cpp \
    randomQ.cpp \
    database.cpp \
    trustknowledge.cpp \
    trustDecision.cpp \
    average.cpp \
    logdatabase.cpp \
    trustreputational.cpp \
    guitableforchart.cpp \
    guiChart.cpp \
    evaluation.cpp

HEADERS += \
        mainwindow.h \
    interaction.h \
    randomQ.h \
    database.h \
    structs.h \
    trustknowledge.h \
    trustDecision.h \
    average.h \
    logdatabase.h \
    trustreputational.h \
    guitableforchart.h \
    guiChart.h \
    evaluation.h

FORMS += \
        mainwindow.ui
