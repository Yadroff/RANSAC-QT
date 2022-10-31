#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T23:24:55
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++17
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++17

TARGET = interaction-example
TEMPLATE = app

SOURCES += main.cpp\
    calculator.cpp \
    calculatormanager.cpp \
        mainwindow.cpp \
         ../QCustomPlot/qcustomplot.cpp

HEADERS  += mainwindow.h \
         ../QCustomPlot/qcustomplot.h \
         calculator.h \
         calculatormanager.h

FORMS    += mainwindow.ui

