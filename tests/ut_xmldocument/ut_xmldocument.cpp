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
#include "ut_xmldocument.h"
#include "test_support.h"
#include "defines.h"
#include "editortabwidget.h"
#include "ebtextedit.h"


void Ut_XmlDocument::init()
{
  //create object

}

void Ut_XmlDocument::cleanup()
{
  //deleate object
}

void Ut_XmlDocument::testLoadXml_data()
{

    QTest::addColumn<enum EditorView::Type>("ViewType");
    //QTest::newRow("Default") << EditorView::Default;
    QTest::newRow("XmlSource") << EditorView::XmlSource;
    QTest::newRow("Text") << EditorView::Text;
    //QTest::newRow("RichFormat") << EditorView::RichFormat;
    QTest::newRow("OnlyPlainData") << EditorView::OnlyPlainData;
}

void Ut_XmlDocument::testLoadXml()
{
    QString sTestedInput("./test.eb");
    QVERIFY(QFileInfo(sTestedInput).size()>0);

    QFETCH( enum EditorView::Type, ViewType );
    EBTextEdit * m_textEdit = new EBTextEdit(sTestedInput, ViewType);
    QVERIFY(m_textEdit != NULL);

    //do the load
    bool bReadOk = m_textEdit->readFile();
    QVERIFY(bReadOk == true);
    //todo this can be used for combinations switching the views but otherewise they have different sizes
    //qDebug()<< "document size" << m_textEdit->document()->toPlainText().size();
    //QCOMPARE(double(QFileInfo(sTestedInput).size()), double(m_textEdit->document()->toPlainText().size()));

    QString sTestedOutput = QString("./test") + QString(QTest::currentDataTag()) + QString(".ebg");
    bool bWriteOk = m_textEdit->writeFile(sTestedOutput);
    QVERIFY(bWriteOk == true);
    QVERIFY(QFileInfo(sTestedOutput).size()>0);

    //first compare size
    QCOMPARE(QFileInfo(sTestedOutput).size(), QFileInfo(sTestedInput).size());

    //than compare content - without LE
    QVERIFY (QProcess::execute("diff", QStringList() << qPrintable(sTestedInput) << qPrintable(sTestedOutput) << "--line-format=")==0);
    //than compare content - bite by bite
    QVERIFY(compareFiles(sTestedOutput, sTestedInput));

    delete m_textEdit;
}

QTEST_MAIN(Ut_XmlDocument)
