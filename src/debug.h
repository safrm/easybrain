/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://safrm.net/easybrain
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

#ifndef DEBUG_H
#define DEBUG_H
#include <qglobal.h>
class QStringList;
class QPlainTextEdit;

class Debug
{
public:
    Debug();
    static void mMessageHandler(QtMsgType type, const char *msg);
    static void createDebugWindow();
    static void closeDebugWindow();
    static void setVisibleDebugWindow(bool bVisible);


private:
    static QStringList * debugStartupBuffer;
    static QPlainTextEdit* debugTextEdit;
};

#endif // DEBUG_H
