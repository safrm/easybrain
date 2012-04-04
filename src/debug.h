/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
