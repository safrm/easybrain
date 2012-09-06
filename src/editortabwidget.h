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

#ifndef EDITORTABWIDGET_H
#define EDITORTABWIDGET_H
#include <QTabWidget>
#include "defines.h"

class QString;
class EBTextEdit;
class QMenu;
class QActionGroup;
class QPrinter;
class EditorTabWidget_private;

namespace EditorView  {
    Type Int2Enum(int iType);
    Type String2Enum(const QString & sType);
    const QString  Enum2String(Type  eType);
}

class EditorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit EditorTabWidget(QWidget *parent = 0);


    bool addOpenItem(const QString &sFileName, EditorView::Type viewType = EditorView::Default);
    bool saveAsCurrentItem(const QString & sFileName);
    QString currentTabFileName() const;
    EBTextEdit* currentTextEdit();
    EBTextEdit* textEditByFilename(const QString & sFilename) const;
    bool isFileOpen(const QString & sFilename) const;
    QActionGroup* changeViewActionGroup();
    QPrinter* printer();

signals:
    void currentViewChanged(EditorView::Type type);
    void refreshFileRow(const QString & sFileName, const QString & sItemName);

public slots:
      bool addNewItem();
      void undo();
      void redo();
      void cut();
      void copy();
      void paste();
      void clear();
      void selectAll();
      bool saveCurrentItem();
      bool saveAll();
      bool closeTab(QString sFileName = QString());
      void closeOtherTabs();
      void insetShortTime();
      void insetDate();
      void printCurrentItem();
      void copyFullFileName();
      void openFileFolder();
      void switchView(QAction* action);
      void enableToolbars(int index);
      void updateViewActions(EditorView::Type type);

protected :
  virtual void mousePressEvent(QMouseEvent *);
  virtual void contextMenuEvent ( QContextMenuEvent * event );

private:
    QMenu* rightClickMenu();

private:
    EditorTabWidget_private* d;
};

#endif // EDITORTABWIDGET_H
