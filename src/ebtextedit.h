/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
