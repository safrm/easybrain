#binary and tests
CONFIG      += debug
#CONFIG      -= debug
debug:DEFINES += DEBUG
debug:message("DEBUG")

unix:APP_DATE_SYS=$$system(date +'\"%Y%m%d_%H%M\"')
#win32:APP_DATE_SYS=$$system(date /T)
#message("APP_DATE_SYS = " $$APP_DATE_SYS)
DEFINES += APP_DATE=\\\"$$APP_DATE_SYS\\\"

win32:APP_VERSION_FULL=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags  --dirty=* 2> /dev/null  \")
unix:APP_VERSION_FULL=$$system(git describe --tags  --dirty=* 2> /dev/null)
DEFINES += APP_VERSION_FULL=\\\"$$APP_VERSION_FULL\\\"
message("APP_VERSION_FULL = " $$APP_VERSION_FULL)


win32:APP_VERSION_SHORT=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags --abbrev=0 2> /dev/null  \")
unix:APP_VERSION_SHORT=$$system(git describe --tags --abbrev=0 2> /dev/null)
DEFINES += APP_VERSION_SHORT=\\\"$$APP_VERSION_SHORT\\\"
message("APP_VERSION_SHORT = " $$APP_VERSION_SHORT)



#QT info
message("QT_VERSION = " $${QT_VERSION})
DEFINES += QMAKE_QMAKE=\\\"$${QMAKE_QMAKE}\\\"
message("QMAKE_QMAKE = " $${QMAKE_QMAKE})
DEFINES += QMAKE_QT_DLL=\\\"$${QMAKE_QT_DLL}\\\"
message("QMAKE_QT_DLL = " $${QMAKE_QT_DLL})

DEFINES += APPLICATION_NAME=\\\"${QMAKE_TARGET}\\\"

QT          = core gui network xml

# All generated files goes to the same directory
win32:INSTALL_ROOT= /progs/easybrain
