/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://safrm.net/easybrain
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

#include <QDir>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication> //clipboard
#include <QClipboard>
#include <QHeaderView>
#include <QProcess>
#include <QDateTime>

#include "filelistwidget.h"
#include "mainwindow.h"
#include "dataowner.h"
#include "editortabwidget.h"
#include "ebtextedit.h"

FileListWidget::FileListWidget(QWidget *parent, ItemType type)
        : QTreeWidget(parent),
          m_Type(type)
{
    //Q_D(FileListWidget);
    //d->setup();
    setIndentation(0); //align to left
    setColumnCount(CountOfCols);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    if (m_Type != Backup)
        setSelectionMode(QAbstractItemView::ExtendedSelection);

    if (type == Backup)
        setHeaderLabels(QStringList() << "Backup" <<  "modified" << "size [b]" << "full file path");
    else
        setHeaderLabels(QStringList() << "Name"  << "modified"   << "size [b]" << "full file path");

    header()->setResizeMode(QHeaderView::Interactive);
    header()->resizeSection(static_cast<int>(Name), 155);
    header()->resizeSection(static_cast<int>(Modified), 48);
    header()->resizeSection(static_cast<int>(Size), 35);
    header()->setResizeMode(static_cast<int>(FullFilename), QHeaderView::ResizeToContents);

    m_RightClickPopupMenu = new QMenu(this);
    m_openAct = new QAction(QIcon(":/images/open.svg"),tr("Open files in default view"),this);
    m_openAsSourceAct = new QAction(QIcon(":/images/open.svg"),tr("Open files in source view"),this);
    m_copyFilenameAct = new QAction(QIcon(":/images/copy.svg"),tr("Copy full filename"),this);
    m_deleteAct = new QAction(QIcon(":/images/delete.svg"),tr("Delete files"),this);
    m_mailFileAct =  new QAction(QIcon(":/images/mail_file.svg"),tr("Send files by email"),this);

    m_RightClickPopupMenu->addAction(m_openAct);
    if (type != Backup)
        m_RightClickPopupMenu->addAction(m_openAsSourceAct);
    m_RightClickPopupMenu->addAction(m_copyFilenameAct);
    m_RightClickPopupMenu->addAction(m_mailFileAct);
    m_RightClickPopupMenu->addSeparator();
    m_RightClickPopupMenu->addAction(m_deleteAct);

    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
               this,SLOT(openFile(QTreeWidgetItem* , int)));
}

void FileListWidget::openFile(QTreeWidgetItem * item, int )
{
    if (m_Type == Backup)
        openBackupFile(item->text(FullFilename));
    else
        MW->editorTab()->addOpenItem(item->text(FullFilename));
}
void  FileListWidget::openBackupFile(const QString &sFileName)
{
    QProcess proc;
    const QString program = "/usr/bin/gnome-open";
    const QStringList arguments = QStringList() << sFileName;
    proc.setWorkingDirectory(DO->dataFolder());
    proc.execute(program, arguments);
    proc.waitForStarted();
}
void FileListWidget::contextMenuEvent ( QContextMenuEvent * event )
{
   const QAction* selectedAction = m_RightClickPopupMenu->exec(event->globalPos());
   const QTreeWidgetItem* item = itemAt(event->pos());
   if (selectedAction && item) {
       if (selectedAction == m_openAct) {
          if (m_Type == Backup)
             openBackupFile(item->text(FullFilename));
           else {
                foreach(const QTreeWidgetItem* selectedItem,selectedItems())
                    MW->editorTab()->addOpenItem(selectedItem->text(FullFilename));
           }
       }
       else if (selectedAction == m_openAsSourceAct) {
           if (m_Type == Backup)
               Q_ASSERT(0); ///should not happen
            else {
                 foreach(const QTreeWidgetItem* selectedItem,selectedItems())
                     MW->editorTab()->addOpenItem(selectedItem->text(FullFilename), EditorView::XmlSource);
            }
       }
       else if (selectedAction == m_copyFilenameAct)
           QApplication::clipboard()->setText(item->text(FullFilename), QClipboard::Clipboard);
       else if (selectedAction == m_deleteAct) {
          //QList<QTreeWidgetItem *> toDelete = findItems(item->text(FullFilename),Qt::MatchExactly,static_cast<int>(FullFilename));
          foreach(QTreeWidgetItem* selectedItem,selectedItems()) {
             if (MW->deleteFile(selectedItem->text(FullFilename)))
                 takeTopLevelItem(indexOfTopLevelItem(selectedItem));
              else
                  qCritical("cannot delete:%s", qPrintable(selectedItem->text(FullFilename)));
          }
       } else if (selectedAction == m_mailFileAct) {
           MW->emailFile(item->text(FullFilename));
        /*   QString sFiles("");
           foreach(const QTreeWidgetItem* selectedItem,selectedItems()) {
               sFiles += selectedItem->text(FullFilename);
               sFiles += " , ";
           }
           MW->emailFile(sFiles);
           */
       }
       else
           qCritical("Unknown action");
      }
}

//updated from EditorTabWidget::saveCurrentItem()
void FileListWidget::updateItemRow(const QString & sFileName, const QString & sItemName)
{
  QList<QTreeWidgetItem *> itemList = findItems(sFileName, Qt::MatchExactly,static_cast<int>(FullFilename));
  Q_ASSERT(itemList.count()==1); //not 2 or 0
  if (itemList.count()!=1)
      return;

  QFileInfo fileInfo(sFileName);
  itemList[0]->setText(Name, sItemName);
  itemList[0]->setText(Modified, fileInfo.lastModified().toString("hh:mm"));
  itemList[0]->setText(Size, QString::number(fileInfo.size()));
  itemList[0]->setToolTip(0,sItemName);
  //sort?
}
