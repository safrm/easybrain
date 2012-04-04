/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
