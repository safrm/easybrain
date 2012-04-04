/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
