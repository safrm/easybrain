/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef VERSION_H
#define VERSION_H
class QString;
#define COMPILE_TIME_YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
+ (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

class Version {

  public:
    static QString compileDate();
    static QString getFullString();
    static QString getShortString();
    static const QString COMPILE_DATE_FORMAT;
};



#endif // VERSION_H
