#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T00:19:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ARK-Grade
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

RC_FILE += ark.rc
		
HEADERS  += mainwindow.h \
    ark.rc

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
	
