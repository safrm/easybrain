/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
