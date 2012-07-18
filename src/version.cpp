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
       sVersion.sprintf("%s_%s%s%s%s-(compiled:%s)",APPLICATION_NAME,APP_VERSION_FULL,APP_DATE,sz_Platform,sz_BuildType,compileDate().toLatin1().data());
    return sVersion;
}

QString Version::getShortString()
{
    return QString(APP_VERSION_SHORT);
}

