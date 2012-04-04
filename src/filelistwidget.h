/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QTreeWidget>
#include "defines.h"
class FileListWidgetPrivate;
class QTreeWidgetItem;
class QPoint;

class FileListWidget : public QTreeWidget
{
   Q_OBJECT

public:
   enum Col {
         Name         = 0,
         Modified     = 1,
         Size         = 2,
         FullFilename = 3,

         CountOfCols
   };
   explicit FileListWidget(QWidget *parent,ItemType type);
protected:
   virtual void contextMenuEvent ( QContextMenuEvent * event ) ;
   void openBackupFile(const QString &sFileName);
public slots:
    void openFile(QTreeWidgetItem * item, int column);
    void updateItemRow(const QString &sFileName, const QString & sItemName);
private:
   Q_DECLARE_PRIVATE(FileListWidget);
   ItemType m_Type;
   QMenu* m_RightClickPopupMenu;
   QAction* m_openAct;
   QAction* m_openAsSourceAct;
   QAction* m_copyFilenameAct;
   QAction* m_deleteAct;
   QAction* m_mailFileAct;
};

#endif // FILELISTWIDGET_H
