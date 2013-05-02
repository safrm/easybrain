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

#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QApplication>

#include "ebtextedit.h"
#include "xmlhightlighter.h"
#include "dataowner.h"


EBTextEdit::EBTextEdit(const QString  & sFullFileName, EditorView::Type viewType, EditorTabWidget *parent) :
    QPlainTextEdit(parent),
    m_FullFileName(sFullFileName),
    m_ViewType(viewType),
    m_EditorTabWidget(parent),
    m_Highlighter(NULL)
{

    setBaseSize(50,50);
    setTabStopWidth(16);
    if (viewType == EditorView::XmlSource)
        m_Highlighter = new XmlHighlighter(document());
}

EBTextEdit::~EBTextEdit()
{
    if (m_Highlighter) {
        delete m_Highlighter;  m_Highlighter = NULL;
    }
}

bool EBTextEdit::readFile()
{
    qDebug()<<  "EBTextEdit::readFile" << qPrintable(m_FullFileName);
    QFile file(m_FullFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;
    if (m_ViewType == EditorView::XmlSource) {
        qDebug() << "file.read(SOURCE): "<< m_FullFileName;
        setPlainText(file.readAll());
    } else if (m_ViewType == EditorView::Text || m_ViewType == EditorView::OnlyPlainData ) {
        qDebug() << "file.read(DOM): "<< m_FullFileName;
        QDomDocument doc( m_FullFileName );
        QString errotMsg("");
        int iErrorLine(0), iErrorCol(0);
        if ( !doc.setContent( &file,false, &errotMsg, &iErrorLine, &iErrorCol))
        {
          file.close();
          qCritical()<< "QDomDocument::setContent: " <<  errotMsg << " Line " << iErrorLine << " column: " << iErrorCol;
          return false;
        }
        file.close();

        QString tagName("");
        QDomElement docElem = doc.documentElement();
        QDomNode n = docElem.firstChild();

        //we take only data
        if(m_ViewType == EditorView::OnlyPlainData) {
            QString sData = docElem.elementsByTagName(DataOwnerSingl::XmlKeywords::KEYWORD_DATA).at(0).toElement().text();
            if(sData.startsWith("\n"))
               sData = sData.mid(1);
            appendPlainText(sData);
        }
        else {
            while(!n.isNull()) {
                QDomElement e = n.toElement();
                if (!e.isNull()) {
                    //qDebug()<< qPrintable(e.tagName())  << " : " << qPrintable(e.text());
                    tagName =  DataOwnerSingl::XmlKeywords::convertKeywordXml2Txt(e.tagName());
                    appendPlainText(tagName + qPrintable(e.text()));
                }
                n = n.nextSibling();
            }
        }
    }
    else
        qCritical("More views are not implemented yet.");
    setDocumentTitle(m_FullFileName);
    return true;
}

bool EBTextEdit::writeFile(QString sFileName )
{
    Q_ASSERT(m_FullFileName.size());
    //save or save as?
    if (sFileName.isEmpty())
        sFileName = m_FullFileName;

    QDomDocument xmlDocument; //has to be first because of EditorView::OnlyPlainData reads it
    QFile fileWrite(sFileName);
    QIODevice::OpenMode flags = QFile::WriteOnly | QFile::Text;
    if (m_ViewType == EditorView::OnlyPlainData) {
        QFile fileRead(m_FullFileName); //read always from source/can be the same as fileWrite
        if (!fileRead.open(QFile::ReadOnly| QFile::Text))
            return false;
        QString errotMsg("");
        int iErrorLine(0), iErrorCol(0);
        if ( !xmlDocument.setContent( &fileRead,false, &errotMsg, &iErrorLine, &iErrorCol))
        {
            fileRead.close();
            qCritical()<< "QDomDocument::setContent: " <<  errotMsg << " Line " << iErrorLine << " column: " << iErrorCol;
            return false;
        }
        fileRead.close();
    }
    if (!fileWrite.open(flags))
        return false;

    QTextStream out(&fileWrite);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    if (m_ViewType == EditorView::XmlSource) {
        qDebug() << "file.write(SOURCE): "<< sFileName;
        out << toPlainText();
    } else if (m_ViewType == EditorView::Text) {
        qDebug() << "file.write(DOM): "<< sFileName;
        QDomElement root = xmlDocument.createElement(DataOwnerSingl::XmlKeywords::KEYWORD_ITEM );
        xmlDocument.appendChild( root );


        //actual reading TXT format and saving to QDomDocument
        //1.we divide it to 2 parts - tags and data
        //than we red keyword one by one and if some is missing we use default
        //than we take the data as a rest

        QDomElement tag;
        QTextCursor cursor(document());
        int iDataCursorPosition = document()->end().position(); //default is end of document
        if (!document()->find(DataOwnerSingl::TxtKeywords::KEYWORD_DATA).isNull()) {
            cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::MoveAnchor); //remove txt keyword from selection
            iDataCursorPosition = document()->find(DataOwnerSingl::TxtKeywords::KEYWORD_DATA).position();
            //\n should be part of TxtKeywords::KEYWORD_DATA but it can be deleted..
            if(document()->characterAt(iDataCursorPosition) == QChar('\n') ) //QChar::LineSeparator()
                ++iDataCursorPosition;
        }

        //key = xml, value = txt
        QList< QPair<QString, QString> >::const_iterator i = DataOwnerSingl::XmlKeywords::Keywords.constBegin();
        while (i !=  DataOwnerSingl::XmlKeywords::Keywords.constEnd()) {
            if ((*i).first != DataOwnerSingl::XmlKeywords::KEYWORD_DATA) {
                //tag
                tag = xmlDocument.createElement((*i).first);
                root.appendChild(tag);

                //content (before data)
                QString sText("");
                cursor = document()->find((*i).second);

                if (cursor.position() < iDataCursorPosition ) {
                        //valid record
                        const QTextBlock block = cursor.block();
                        if (block.isValid())
                            sText = block.text();
                        sText.remove(QRegExp("^"+ (*i).second));
                        //qDebug() << "writeFile sText:" << sText;
                    }
                //missing or invalid = empty
                QDomText text = xmlDocument.createTextNode(sText);
                tag.appendChild(text);
            }
            ++i;
        }

    tag = xmlDocument.createElement(DataOwnerSingl::XmlKeywords::KEYWORD_DATA);
    root.appendChild(tag);
    //select from iDataCursorPosition till end - if tag is missing it is empty, with \n correction
    cursor.setPosition(iDataCursorPosition, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    QDomCDATASection data = xmlDocument.createCDATASection(cursor.selectedText());
    tag.appendChild(data);

    //qDebug()<< "save XML:" << xmlDocument.toString();
    out << xmlDocument.toString();
    } else if (m_ViewType == EditorView::OnlyPlainData) {
        QDomNodeList nodeList = xmlDocument.elementsByTagName(DataOwnerSingl::XmlKeywords::KEYWORD_DATA);
        QDomElement data;
        if (nodeList.isEmpty()) {
            data =  xmlDocument.createElement(DataOwnerSingl::XmlKeywords::KEYWORD_DATA);
            xmlDocument.documentElement().appendChild(data);
        }
        else
        {
            data = nodeList.at(0).toElement();
            data.removeChild(data.childNodes().at(0));
        }
        QDomCDATASection dataText = xmlDocument.createCDATASection("\n" + document()->toPlainText());
        data.appendChild(dataText);
        //qDebug()<< "save XML:" << xmlDocument.toString();
        out << xmlDocument.toString();
    } else
        Q_ASSERT(0);
    fileWrite.close();
    QApplication::restoreOverrideCursor();

    return true;
}

QString EBTextEdit::itemNameFromDocument()
{
    QString sItemName("");
    if (m_ViewType == EditorView::XmlSource) {
        //Is there any problem with QTextEdit::toPlainText and QDomDocument::setContent?
        QDomDocument xmlDocument(m_FullFileName);
        QString errotMsg("");
        int iErrorLine(0), iErrorCol(0);
        if (!xmlDocument.setContent(toPlainText(),false, &errotMsg, &iErrorLine, &iErrorCol))
        {
            qCritical()<< "QDomDocument::setContent: " <<  errotMsg << " Line " << iErrorLine << " column: " << iErrorCol;
            return sItemName;
        }
        QDomNodeList nodeList = xmlDocument.elementsByTagName(DataOwnerSingl::XmlKeywords::KEYWORD_NAME);
        if (nodeList.isEmpty())
            return sItemName;
        sItemName =  nodeList.at(0).toElement().text();
    }
    else if (m_ViewType == EditorView::Text) {
        QTextCursor cursor = document()->find(DataOwnerSingl::TxtKeywords::KEYWORD_NAME);
        const QTextBlock block = cursor.block();
        if (!block.isValid())
            return QString("");
        sItemName = block.text();
        sItemName.remove(QRegExp("^"+ DataOwnerSingl::TxtKeywords::KEYWORD_NAME));
        //qDebug() << "itemNameFromDocument:" << sItemName;
    }
    else if(m_ViewType == EditorView::OnlyPlainData) {
        //todo
       sItemName = DataOwnerSingl::XmlKeywords::itemNameFromFile(m_FullFileName);
    } else
        Q_ASSERT(0);
    return sItemName;
}

bool EBTextEdit::changeView(EditorView::Type viewType)
{
    if (viewType == m_ViewType)
        return true;

    //todo4 warning when there are changes ?
    m_EditorTabWidget->saveCurrentItem();

    clear();
    if (viewType == EditorView::XmlSource) {
        m_Highlighter = new XmlHighlighter(document());
    }
    else if (viewType == EditorView::Text || viewType == EditorView::OnlyPlainData ) {
        if (m_Highlighter) {
            delete m_Highlighter;  m_Highlighter = NULL;
        }
    } else
        Q_ASSERT(0); //todo

    m_ViewType = viewType;
    if (!readFile())
        return false;

    return true;
}

EditorView::Type EBTextEdit::currentView()
{
    return  m_ViewType;
}

