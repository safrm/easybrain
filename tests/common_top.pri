#only for tests
include(../common.pri)
SRCDIR = ../../src
STUBSDIR = ../stubs
TESTS_DIR = ../
INCLUDEPATH += $$SRCDIR $$STUBSDIR $$TESTS_DIR
DEPENDPATH = $$INCLUDEPATH
QT += testlib core gui xml
TEMPLATE = app
DEFINES += UNIT_TEST

HEADERS += $$TESTS_DIR/test_support.h
# install tests
#target.path = $$[QT_INSTALL_LIBS]/tests
#INSTALLS += target

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = @ ./$$TARGET
