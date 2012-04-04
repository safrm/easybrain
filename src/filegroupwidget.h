/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef FILEGROUPWIDGET_H
#define FILEGROUPWIDGET_H

#include <QWidget>
#include "defines.h"

class FileGroupWidgetPrivate;
class FileListWidget;

class FileGroupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileGroupWidget(const ProjectString & projectName, QWidget *parent = 0);
    void addNewFile(const QString &sFilename);
    void addNewBackup(const QString &sFilename);

    FileListWidget* workFiles;
    FileListWidget* archiveFiles;
    FileListWidget* backupFiles;

private:
   Q_DECLARE_PRIVATE(FileGroupWidget)

signals:
    void refreshFileRow(const QString & sFileName, const QString & sItemName);
public slots:

};

#endif // FILEGROUPWIDGET_H
