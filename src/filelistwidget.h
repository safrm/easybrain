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
