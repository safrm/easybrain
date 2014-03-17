/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
** Source: http://safrm.net/projects/easybrain
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QDebug>
#include <QTextCodec>
#include "mainwindow.h"
#include "version.h"
#include "debug.h"

void print_version()
{
    printf("%s \n", Version::getShortString().toLatin1().data());
    exit(0);
}

void print_help()
{
    printf("########################################\n");
    printf("-h  --help      print command line usage\n");
    printf("-t  --tray      start minimized in tray\n");
    printf("-V  --version   print version\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    qInstallMsgHandler(Debug::mMessageHandler);
    qDebug() << "######" << Version::getFullString();
    //print cmdline options
    for(int i=1; i < argc; ++i) {
      qDebug() << argv[i];
    }
    //qDebug() << "__TIMESTAMP__:" << __TIMESTAMP__;
    //qDebug() << "COMPILE_TIMESTAMP:" << COMPILE_TIMESTAMP;
    //qDebug() << "COMPILE_TIME_YEAR:" << COMPILE_TIME_YEAR;
#ifdef _linux
    qDebug() << "QT complile:" << QT_VERSION_STR << QMAKE_QMAKE << QMAKE_QT_DLL;
#endif //_linux
    qDebug() << "QT runtime:" << qVersion();

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontShowIconsInMenus,false);
    // Cmdline arguments
    //m_cheating = qApp->arguments().contains("-cheat");

    QString sParam("");
    bool bStartInTray(false);
    for (int i = 0; i < argc; ++i) {
        sParam = qApp->arguments().at(i);
        if (sParam == "-t"  || sParam == "--tray") { //start in tray
            bStartInTray = true;
        } else if (sParam == "-V" || sParam == "--version") {
            print_version();
        } else if (sParam == "-h" || sParam == "--help") {
            print_help();
        }

    }

    MainWindow* mainWin = new MainWindow();
    int iWidth(640),iHeight(480); //int iWidth(200),iHeight(100);
    int iXleft = QApplication::desktop()->screenGeometry().size().width()- iWidth;
    int iYup = QApplication::desktop()->screenGeometry().size().height() - iHeight;

    mainWin->setGeometry(iXleft,iYup,iWidth,iHeight);
    if (!bStartInTray)
        mainWin->show();
    return app.exec();
}
