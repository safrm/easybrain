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

#include <QSplitter>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDateTime>

#include "filegroupwidget.h"
#include "filelistwidget.h"
#include "dataowner.h"

FileGroupWidget::FileGroupWidget(const ProjectString & projectName, QWidget *parent) :
    QWidget(parent)
{
    workFiles = new FileListWidget(this, Working);
    QFileInfoList workList = DO->projectWorkingFiles(projectName);
    foreach(QFileInfo fileInfo, workList) {
        QString sItemName = DataOwnerSingl::XmlKeywords::itemNameFromFile(fileInfo.filePath());
        QTreeWidgetItem* line = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << sItemName
                     <<  fileInfo.lastModified().toString("hh:mm") << QString::number(fileInfo.size()) << fileInfo.filePath());
        line->setToolTip(0,sItemName);
        workFiles->addTopLevelItem(line);
   }
    archiveFiles = new FileListWidget(this,Archive);
    QFileInfoList archiveList = DO->projectArchiveFiles(projectName);

    foreach(QFileInfo fileInfo, archiveList) {
        QString sItemName = DataOwnerSingl::XmlKeywords::itemNameFromFile(fileInfo.filePath());
        QTreeWidgetItem* line = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << sItemName
                     <<  fileInfo.lastModified().toString("hh:mm") << QString::number(fileInfo.size()) << fileInfo.filePath());
        line->setToolTip(0,sItemName);
        archiveFiles->addTopLevelItem(line);
   }

    backupFiles = new FileListWidget(this,Backup);
    QFileInfoList backupList = DO->projectBackupFiles(projectName);
    foreach(QFileInfo fileInfo, backupList) {
        QString sItemName = fileInfo.lastModified().toString("yyyy_MM_dd_hh:mm");
        QTreeWidgetItem* line = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << sItemName
                     <<  fileInfo.lastModified().toString("hh:mm") << QString::number(fileInfo.size()) << fileInfo.filePath());
        line->setToolTip(0,sItemName);
        backupFiles->addTopLevelItem(line);
    }

    QVBoxLayout * mainLayout= new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(workFiles);
    mainLayout->addWidget(archiveFiles);
    mainLayout->addWidget(backupFiles);
    setLayout(mainLayout);

    connect(this, SIGNAL(refreshFileRow(const QString & , const QString & )), workFiles, SLOT(updateItemRow(const QString & , const QString & )));
}
void FileGroupWidget::addNewFile(const QString &sFilename)
{
    QFileInfo fileInfo(sFilename);
    QString sItemName =  DataOwnerSingl::XmlKeywords::itemNameFromFile(sFilename);
    QTreeWidgetItem* line = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << sItemName
                 <<  fileInfo.lastModified().toString("hh:mm") << QString::number(fileInfo.size()) << fileInfo.filePath());
    line->setToolTip(0,sItemName);
    workFiles->insertTopLevelItem(0,line);
}
void FileGroupWidget::addNewBackup(const QString &sFilename)
{
    QFileInfo fileInfo(sFilename);
    QString sItemName = fileInfo.lastModified().toString("yyyy_MM_dd_hh:mm");
    QTreeWidgetItem* line = new QTreeWidgetItem((QTreeWidget*)0, QStringList() << sItemName
                 <<  fileInfo.lastModified().toString("hh:mm") << QString::number(fileInfo.size()) << fileInfo.filePath());
    line->setToolTip(0,sItemName);
    backupFiles->insertTopLevelItem(0,line);
}
