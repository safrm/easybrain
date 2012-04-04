/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
