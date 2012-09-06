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

#ifndef DATAOWNER_H
#define DATAOWNER_H
#include <QStringList>
#include <QFileInfoList>
#include "defines.h"
class DataOwnerSingl_private;
class ProjectSettings;

#define DO DataOwnerSingl::instance()
class DataOwnerSingl
{
public:
    static DataOwnerSingl* instance();
    QString dataFolder();
    QString newItemFile();
    ProjectString currentProject();
    QString projectFolder(const ProjectString & projectName = QString("")); //empty is current
    QFileInfoList projectWorkingFiles(const ProjectString & projectName = QString(""));  //empty is current
    QFileInfoList projectArchiveFiles(const ProjectString & projectName = QString("")); //empty is current
    QFileInfoList projectBackupFiles(const ProjectString & projectName = QString(""));
    int projectFilesCount(const ProjectString & projectName = QString("")); //empty is current
    TimeStampString now(QString sFormat = TIMESTAMP_FORMAT);
    void saveCurrentProjectSettings();

private:
  explicit DataOwnerSingl();
  static DataOwnerSingl* m_instancePtr;
  Q_DISABLE_COPY(DataOwnerSingl)

  static const QString DefaultItemTemplate;

public:
  static const QString WORK_DIR_NAME;
  static const QString ARCHIVE_DIR_NAME;
  static const QString TEMP_DIR_NAME;
  static const QString BACKUP_DIR_NAME;
  static const QString TIMESTAMP_FORMAT;
  static const QString TIME_SIMPLE_FORMAT;
  static const QString DATE_SIMPLE_FORMAT;
  static const QString FILE_EXT_EB;
  static const QString FILE_EXT_TARBZ2;
  static const QString FILE_EXT_TXT;
  static const QString FILE_EXT_ZIP;

  class TxtKeywords {
  public:
          static const QString KEYWORD_NAME;
          static const QString KEYWORD_DATE;
          static const QString KEYWORD_GROUPS;
          static const QString KEYWORD_PRIORITY;
          static const QString KEYWORD_TAGS;
          static const QString KEYWORD_DATA;
  };
  class XmlKeywords {
  public:
      static const QString KEYWORD_ITEM;
      static const QString KEYWORD_NAME;
      static const QString KEYWORD_DATE;
      static const QString KEYWORD_GROUPS;
      static const QString KEYWORD_PRIORITY;
      static const QString KEYWORD_TAGS;
      static const QString KEYWORD_DATA;
      static QString itemNameFromFile(const QString &sFileName);
      static QString convertKeywordXml2Txt(const QString & xmlTag);
      static const QList< QPair<QString, QString> > Keywords;
  };

protected:
    DataOwnerSingl_private *const d_ptr;
    ProjectString sCurrentProject;
    QString m_DataFolder;
    ProjectSettings* currentProjectSettings;
};

#endif // DATAOWNER_H
