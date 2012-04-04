/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
    //projectFoldersExistCheck(projectFolder + QDir::separator() + projectName);
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
    //setValue("LastSynchTime",sLastSynchTime);
    //setValue("BackupPassword",sBackupPassword);

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
