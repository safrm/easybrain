/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef UT_XMLDOCUMENT_H
#define UT_XMLDOCUMENT_H

#include <QtTest/QtTest>
#include <QObject>

//class EBTextEdit;
class Ut_XmlDocument : public QObject
{
    Q_OBJECT

private:
//   EBTextEdit *m_textEdit;
private slots:
    void init();
    void cleanup();

    void testLoadXml_data();
    void testLoadXml();

};

#endif // UT_XMLDOCUMENT_H

