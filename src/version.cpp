/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#include "version.h"
#include <QString>
#include <QLocale>
#include <QDate>

#ifndef APP_DATE
#define APP_DATE "1/1/1970"
#endif

#ifndef APP_VERSION_FULL
#define APP_VERSION_FULL "local-dev"
#endif
#ifndef APP_VERSION_SHORT
#define APP_VERSION_SHORT "local-dev"
#endif

const QString Version::COMPILE_DATE_FORMAT = "yyyy_MM_dd";

#ifdef _WIN32
 const static char sz_Platform[] ="-W";
#elif __linux
 const static char sz_Platform[] = "-L";
#else
 const static char sz_Platform[] = "-?";
#endif

#ifdef DEBUG
 const static char sz_BuildType[] ="-D";
#else
 const static char sz_BuildType[] = "-R";
#endif

//#define COMPILE_TIMESTAMP ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10
//+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

QString Version::compileDate()
{
    return QLocale(QLocale::C).toDate(QString(__DATE__).simplified(), QLatin1String("MMM d yyyy")).toString(COMPILE_DATE_FORMAT);
}

QString Version::getFullString()
{
    static QString sVersion("");
    if (!sVersion.length())
       sVersion.sprintf("%s_%s_%s%s%s-(compiled:%s)",APPLICATION_NAME,APP_VERSION_FULL,APP_DATE,sz_Platform,sz_BuildType,compileDate().toLatin1().data());
    return sVersion;
}

QString Version::getShortString()
{
    return QString(APP_VERSION_SHORT);
}

