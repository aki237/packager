#-------------------------------------------------
#
# Project created by QtCreator 2017-05-09T10:20:02
#
#-------------------------------------------------
include (qmarkdowntextedit/qmarkdowntextedit.pri)
QT       += core gui
LIBS     += -lmagic

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = packager
TEMPLATE = app


SOURCES += main.cpp\
        pwindow.cpp \
    DibbaReader.cpp \
    DibbaWriter.cpp \
    File.cpp \
    filelist.cpp \
    newfiledialog.cpp \
    pabout.cpp

HEADERS  += pwindow.h \
    Dibba.hpp \
    filelist.h \
    newfiledialog.h \
    basename.hpp \
    pabout.h \
    file_utils.h

FORMS    += pwindow.ui

DISTFILES += \
    packager.pro.user

RESOURCES += \
    res.qrc
