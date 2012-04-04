#author: Miroslav.Safr@tieto.com 
include(../common_top.pri)
TARGET = ut_xmldocument


SOURCES += \
    $$STUBSDIR/settings.cpp \
    $$STUBSDIR/project.cpp \
    $$STUBSDIR/mainwindow.cpp \
    $$SRCDIR/filegroupwidget.cpp \
    $$SRCDIR/filelistwidget.cpp \
    $$SRCDIR/dataowner.cpp \
    $$SRCDIR/ebtextedit.cpp \
    $$SRCDIR/editortabwidget.cpp \
    $$SRCDIR/xmlhightlighter.cpp \
    ut_xmldocument.cpp


HEADERS += \
    ut_xmldocument.h \
    $$SRCDIR/defines.h \
    $$SRCDIR/dataowner.h \
    $$SRCDIR/editortabwidget.h \
    $$SRCDIR/ebtextedit.h \
    $$SRCDIR/settings.h \
    $$SRCDIR/xmlhightlighter.h \
    $$SRCDIR/project.h \
    $$SRCDIR/filegroupwidget.h \
    $$SRCDIR/mainwindow.h  \
    $$SRCDIR/filelistwidget.h

QMAKE_CLEAN += $(TARGET) *.ebg

