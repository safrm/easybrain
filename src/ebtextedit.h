/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
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

#ifndef EBTEXTEDIT_H
#define EBTEXTEDIT_H

#include <QPlainTextEdit>
#include "editortabwidget.h"

class XmlHighlighter;
class EBTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EBTextEdit(const QString  & sFullFileName, EditorView::Type viewType, EditorTabWidget *parent = 0);
    virtual ~EBTextEdit();
    EditorView::Type  currentView();
    bool readFile();
    bool writeFile(QString sFileName = QString(""));
    QString itemNameFromDocument();


signals:
    
public slots:
    bool changeView(EditorView::Type viewType);

private:
    QString m_FullFileName;
    EditorView::Type m_ViewType;
    EditorTabWidget* m_EditorTabWidget;
    XmlHighlighter *m_Highlighter;
};

#endif // EBTEXTEDIT_H
