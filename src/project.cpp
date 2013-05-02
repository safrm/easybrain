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

#include <QDebug>
#include <QDir>
#include "project.h"
#include "dataowner.h"

const QString ProjectSettings::PROJECT_INI_FILENAME("project.ini") ;

ProjectSettings::ProjectSettings(const ProjectString & projectName, const QString & projectFolder) :
        QSettings(projectFolder + QDir::separator() + PROJECT_INI_FILENAME ,QSettings::IniFormat),
  sProjectName(projectName)
{
    projectFoldersExistCheck(projectFolder + QDir::separator() + projectName);
}

bool ProjectSettings::load()
{

    sLastSynchTime = value("LastSynchTime", "").toString();
    qDebug() << " project/LastSynchTime: " << qPrintable(sLastSynchTime);
    sBackupPassword = value("BackupPassword", "easybrain").toString();

    return true;
}

bool ProjectSettings::save()
{
    setValue("LastSynchTime",sLastSynchTime);
    setValue("BackupPassword",sBackupPassword);

    return true;
}

void ProjectSettings::projectFoldersExistCheck(const QString & projectFolder)
{
    Q_ASSERT(projectFolder.size());
    if (!QFileInfo(projectFolder).exists()) {
        qCritical() << projectFolder << "does not exist - created" ;
        QDir::current().mkpath(projectFolder);
        if (!QFileInfo(projectFolder).isWritable()) {
           qCritical() << projectFolder << "Project folder not accassible";
           return;
       }
    }

    QString sFolder = projectFolder + QDir::separator() + DataOwnerSingl::WORK_DIR_NAME;
    if (!QFileInfo(sFolder).exists()) {
        qCritical() << sFolder << "does not exist - created" ;
        QDir::current().mkpath(sFolder);
    }

    sFolder = projectFolder + QDir::separator() + DataOwnerSingl::ARCHIVE_DIR_NAME;
    if (!QFileInfo(sFolder).exists()) {
        qCritical() << sFolder << "does not exist - created" ;
        QDir::current().mkpath(sFolder);
    }

    sFolder = projectFolder + QDir::separator() + DataOwnerSingl::TEMP_DIR_NAME;
    if (!QFileInfo(sFolder).exists()) {
        qCritical() << sFolder << "does not exist - created" ;
        QDir::current().mkpath(sFolder);
    }
    sFolder = projectFolder + QDir::separator() + DataOwnerSingl::BACKUP_DIR_NAME;
    if (!QFileInfo(sFolder).exists()) {
        qCritical() << sFolder << "does not exist - created" ;
        QDir::current().mkpath(sFolder);
    }
}
