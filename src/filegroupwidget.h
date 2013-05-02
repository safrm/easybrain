/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://easybrain.safrm.net/
** Source: http://safrm.net/projects/easybrain
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
