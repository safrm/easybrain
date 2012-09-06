/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
** Source: https://github.com/safrm/easybrain
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

#include <stdio.h>  //fprint
#include <stdlib.h> //abort
#include <QStringList>
#include <QPlainTextEdit>
#include <QApplication>
#include <QDesktopWidget>
#include <QIcon>
#include "debug.h"

QStringList* Debug::debugStartupBuffer =  new QStringList();
QPlainTextEdit* Debug::debugTextEdit = NULL;
Debug::Debug()
{
}

void Debug::mMessageHandler(QtMsgType type, const char *msg)
{
#ifdef DEBUG
    if (debugStartupBuffer) {
        *debugStartupBuffer << msg;
    } else {
       debugTextEdit->appendPlainText(msg);
    }
#endif //DEBUG

    fprintf(stderr, "%s\n", msg);

    if (type == QtFatalMsg)
       abort();
}
void Debug::createDebugWindow()
{
#ifdef DEBUG
    debugTextEdit = new QPlainTextEdit(0);
    debugTextEdit->setWindowIcon(QIcon(":/images/debug.svg"));
    debugTextEdit->setWindowTitle("Easybrain-Debug");

    int iWidth(640),iHeight(240);
    int iXleft = QApplication::desktop()->screenGeometry().size().width()- iWidth;
    int iYup = QApplication::desktop()->screenGeometry().size().height() - iHeight - 600;
    debugTextEdit->setGeometry(iXleft,iYup,iWidth,iHeight);

    debugTextEdit->setReadOnly(true);

    foreach(QString sLine, *debugStartupBuffer )
        debugTextEdit->appendPlainText(sLine);
    delete debugStartupBuffer;
    debugStartupBuffer = NULL;

    //debugTextEdit->show();
#endif //DEBUG
}
void Debug::closeDebugWindow()
{
#ifdef DEBUG
  debugTextEdit->close();
  delete debugTextEdit;
  debugTextEdit = NULL;
#endif //DEBUG
}
void Debug::setVisibleDebugWindow(bool bVisible)
{
    if (debugTextEdit) {
        debugTextEdit->setVisible(bVisible);
    }
}
