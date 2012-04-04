TARGET      = easybrain
include(../common.pri)

INCLUDEPATH +=
#MS compiler
#LIBS        += user32.lib
# or
win32:LIBS += -luser32

OBJECTS_DIR = ../build
MOC_DIR     = ../build
RCC_DIR     = ../build
UI_DIR      = ../build

DESTDIR     = ../build

HEADERS     += \ 
    mainwindow.h \
    dataowner.h \
    version.h \
    synchronizer.h \
    project.h \
    defines.h \
    statistics.h \
    filelistwidget.h \
    filegroupwidget.h \
    settings.h \
    debug.h \
    editortabwidget.h \
    xmlhightlighter.h \
    ebtextedit.h \
    aboutdialog.h \
    weblink.h
SOURCES     += main.cpp \
    mainwindow.cpp \
    dataowner.cpp \
    version.cpp \
    synchronizer.cpp \
    project.cpp \
    statistics.cpp \
    filelistwidget.cpp \
    filegroupwidget.cpp \
    settings.cpp \
    debug.cpp \
    editortabwidget.cpp \
    xmlhightlighter.cpp \
    ebtextedit.cpp \
    aboutdialog.cpp \
    weblink.cpp

RESOURCES   += graphics.qrc

INCLUDEPATH +=
#MS compiler
#LIBS        += user32.lib
# or
win32:LIBS += -luser32

INSTALLS    += target
unix:target.path  = /usr/bin
win32:target.path  = /

compiler_clean.commands = -$(DEL_FILE) $(TARGET)

