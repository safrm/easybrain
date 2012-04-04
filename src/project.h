/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#ifndef PROJECT_H
#define PROJECT_H
#include <QSettings>
#include "defines.h"

class ProjectSettings : QSettings {
public:
    explicit ProjectSettings(const ProjectString & projectName, const QString & projectFolder);
    bool load();
    bool save();
private:
    void projectFoldersExistCheck(const QString & projectFolder);
public:
    static const QString PROJECT_INI_FILENAME;

    QString sLastSynchTime;
    QString sBackupPassword;

private:
   ProjectString sProjectName;

};

#endif // PROJECT_H
