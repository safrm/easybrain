include(../common.pri)
CONFIG      -= debug

QMAKE_LINK = @: IGNORE THIS LINE
QMAKE_EXTRA_TARGETS += doc
QMAKE_EXTRA_COMPILER += doc

doc.target        = htmlpages/easybrain.html
doc.CONFIG       += no_check_exist no_link explicit_dependencies disable_check

unix {
    exists( /usr/bin/jss-docs-update ) {
        doc.commands      = jss-docs-update . -sv $$APP_FULL_VERSION_TAG
    } else {
        doc.commands = @echo "Not installed jenkins-support-scripts"
    }
}
doc.input         = $${doc.depends}
doc.output        = $${doc.target}

## Make are doc target build by default If 'make' is executed in doc folder
PRE_TARGETDEPS += $${doc.output}


    INSTALLS    += readme
    unix:readme.path  = /usr/share/doc/easybrain
    win32:readme.path  = /
    readme.files  = ../readme.txt
    #update readme.txt versions
    unix:readme.commands = sed -i".bkp" \'1 s/Easybrain.*/Easybrain $$APP_FULL_VERSION_TAG ($$APP_BUILD_DATE)/\' ../readme.txt && rm -f ../readme.bkp


    INSTALLS    += manpage
	win32:manpage.path  = /
	unix:manpage.path = /usr/share/man/man1/
	manpage.files  = manpages/easybrain.1
	manpage.CONFIG += no_check_exist disable_check

win32 {
	INSTALLS    += htmldocs
	win32:htmldoc.path  = /
	unix:htmldocs.path = /usr/share/doc/easybrain/html/
	htmldocs.files  = htmlpages/*.html
	htmldocs.CONFIG += no_check_exist disable_check

    INSTALLS    += docs
    unix:docs.path  = /usr/share/doc/easybrain
    win32:docs.path  = /
    docs.files  = ../LICENSE.LGPL

}

OTHER_FILES += \
    easybrain.1.xml \
    ../LICENSE.LGPL \
    ../readme.txt

