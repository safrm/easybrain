TEMPLATE    = subdirs
include(common.pri)

SUBDIRS += src \
           tests \
           doc


unix:INSTALLS    += desktop
unix:desktop.path  = /usr/share/applications
unix:desktop.files  = data/easybrain.desktop

INSTALLS    += icon
unix:icon.path   = /usr/share/icons/hicolor/scalable/apps
win32:icon.path  = /
unix:icon.files  = data/easybrain.svg
win32:icon.files  = data/easybrain.ico


win32 {
QTDIR_BIN = $$system(qmake -query QT_INSTALL_BINS)
message($$QTDIR)
INSTALLS    += qtlibraries
qtlibraries.path  = /
qtlibraries.files = \
    $$QTDIR_BIN/mingwm10.dll \
    $$QTDIR_BIN/libgcc_s_dw2-1.dll

  release {
    qtlibraries.files += \    
      $$QTDIR_BIN/QtCore4.dll \
      $$QTDIR_BIN/QtGui4.dll \
      $$QTDIR_BIN/QtSvg4.dll \
      $$QTDIR_BIN/QtXml4.dll \
      $$QTDIR_BIN/QtNetwork4.dll 
  } else {
      qtlibraries.files += \    
      $$QTDIR_BIN/QtCored4.dll \
      $$QTDIR_BIN/QtGuid4.dll \
      $$QTDIR_BIN/QtSvgd4.dll \
      $$QTDIR_BIN/QtXmld4.dll \
      $$QTDIR_BIN/QtNetworkd4.dll 
  }
} 

#
# Targets for debian source and binary package creation
#
debian-src.commands = dpkg-buildpackage -S -r -us -uc -d
debian-bin.commands = dpkg-buildpackage -b -r -uc -d
debian-all.depends = debian-src debian-bin

#
# Clean all but Makefile
#
#compiler_clean.commands = -$(DEL_FILE) $(TARGET)

QMAKE_CLEAN += build/easybrain *.o 
QMAKE_DISTCLEAN += build/easybrain *.o 

QMAKE_EXTRA_TARGETS += debian-all debian-src debian-bin compiler_clean

OTHER_FILES += \
    src/data/definition.txt \
    src/data/empty.txt \
    debian/changelog \
    debian/rules \
    debian/control \
    src/data/empty.xml
