#binary and tests
CONFIG      += debug
#CONFIG      -= debug
debug:DEFINES += DEBUG
debug:message("DEBUG")


#appver, win/lin version system
APP_VERSION_FULL=NA
APP_VERSION_SHORT=NA
unix {
  exists( /usr/bin/appver ) {
    . /usr/bin/appver
  } else {
    APP_DATE_SYS=$$system(date +'\"%Y%m%d_%H%M\"')
    APP_VERSION_FULL=$$system(git describe --tags  --dirty=* 2> /dev/null)
    APP_VERSION_SHORT=$$system(git describe --tags --abbrev=0 2> /dev/null)
  }
}
win32 {
  APP_DATE_SYS=$$system(echo \"%date:~9,4%%date:~6,2%%date:~3,2%_%time:~0,2%%time:~3,2%\")
  exists("C:\\progs\\Git\\bin\\sh.exe") {
      APP_VERSION_FULL=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags  --dirty=* 2> /dev/null  \")
      APP_VERSION_SHORT=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags --abbrev=0 2> /dev/null  \")
   }
}
message("APP_DATE_SYS = " $$APP_DATE_SYS)
DEFINES += APP_DATE=\\\"$$APP_DATE_SYS\\\"
message("APP_VERSION_FULL = " $$APP_VERSION_FULL)
DEFINES += APP_VERSION_FULL=\\\"$$APP_VERSION_FULL\\\"
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
