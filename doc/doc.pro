include(../common.pri)

INSTALLS    += manpage
unix:manpage.path  = /usr/share/man/man1/
win32:manpage.path  = /
manpage.files  = manpages/easybrain.1
#update docs
unix:manpage.commands = jss-docs-update . -sv $$APP_FULL_VERSION_TAG

win32 {
INSTALLS    += htmldoc
win32:docs.path  = /
htmldoc.files  = htmlpages/easybrain.html
}

OTHER_FILES += \
    easybrain.1.xml
