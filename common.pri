#binary and tests
CONFIG      += debug
#CONFIG      -= debug
debug:DEFINES += DEBUG
debug:message("DEBUG")


#appver, win/lin version system
APP_FULL_VERSION_TAG=NA
APP_SHORT_VERSION_TAG=NA
unix {
  exists( /usr/bin/appver ) {
    #APP_BUILD_DATE=$$system(/usr/bin/appver | grep APP_BUILD_DATE | awk -F= '{print $2}')
    #APP_FULL_VERSION_TAG=$$system(/usr/bin/appver | grep APP_FULL_VERSION_TAG | awk -F= '{print $2}')
    #APP_SHORT_VERSION_TAG=$$system(/usr/bin/appver | grep APP_SHORT_VERSION_TAG | awk -F= '{print $2}')
    system(/usr/bin/appver > ./appver.pri)
    include(./appver.pri)
  } else {
    APP_BUILD_DATE=$$system(date +'\"%Y%m%d_%H%M\"')
    APP_FULL_VERSION_TAG=$$system(git describe --tags  --dirty=* 2> /dev/null)
    APP_SHORT_VERSION_TAG=$$system(git describe --tags --abbrev=0 2> /dev/null)
  }
}
win32 {
  APP_BUILD_DATE=$$system(echo \"%date:~9,4%%date:~6,2%%date:~3,2%_%time:~0,2%%time:~3,2%\")
  exists("C:\\progs\\Git\\bin\\sh.exe") {
      APP_FULL_VERSION_TAG=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags  --dirty=* 2> /dev/null  \")
      APP_SHORT_VERSION_TAG=$$system("C:\\progs\\Git\\bin\\sh.exe" -c \"/bin/git describe --tags --abbrev=0 2> /dev/null  \")
   }
}
message("APP_BUILD_DATE = " $$APP_BUILD_DATE)
DEFINES += APP_DATE=\\\"$$APP_BUILD_DATE\\\"
message("APP_FULL_VERSION_TAG = " $$APP_FULL_VERSION_TAG)
DEFINES += APP_FULL_VERSION_TAG=\\\"$$APP_FULL_VERSION_TAG\\\"
DEFINES += APP_SHORT_VERSION_TAG=\\\"$$APP_SHORT_VERSION_TAG\\\"
message("APP_SHORT_VERSION_TAG = " $$APP_SHORT_VERSION_TAG)


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
