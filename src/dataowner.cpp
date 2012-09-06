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

#include <QString>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QXmlStreamReader>

#include "dataowner.h"
#include "settings.h"
#include "project.h"

const QString DataOwnerSingl::WORK_DIR_NAME("work");
const QString DataOwnerSingl::ARCHIVE_DIR_NAME("archive");
const QString DataOwnerSingl::TEMP_DIR_NAME("temp");
const QString DataOwnerSingl::BACKUP_DIR_NAME("backup");
const QString DataOwnerSingl::TIMESTAMP_FORMAT("yyyyMMdd_hhmmss");
const QString DataOwnerSingl::TIME_SIMPLE_FORMAT("hh:mm");
const QString DataOwnerSingl::DATE_SIMPLE_FORMAT("dd.MM.yyyy");
const QString DataOwnerSingl::FILE_EXT_EB(".eb");
const QString DataOwnerSingl::FILE_EXT_TARBZ2(".tar.bz2");
const QString DataOwnerSingl::FILE_EXT_TXT(".txt");
const QString DataOwnerSingl::FILE_EXT_ZIP(".zip");


const QString DataOwnerSingl::TxtKeywords::KEYWORD_NAME("Name: ");
const QString DataOwnerSingl::TxtKeywords::KEYWORD_DATE("Date: ");
const QString DataOwnerSingl::TxtKeywords::KEYWORD_GROUPS("Groups: ");
const QString DataOwnerSingl::TxtKeywords::KEYWORD_PRIORITY("Priority: ");
const QString DataOwnerSingl::TxtKeywords::KEYWORD_TAGS("Tags: ");
const QString DataOwnerSingl::TxtKeywords::KEYWORD_DATA("Data: ");

const QString DataOwnerSingl::XmlKeywords::KEYWORD_ITEM("item");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_NAME("name");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_DATE("date");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_GROUPS("groups");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_PRIORITY("priority");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_TAGS("tags");
const QString DataOwnerSingl::XmlKeywords::KEYWORD_DATA("data");

const QString DataOwnerSingl::DefaultItemTemplate = QString("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>") + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_ITEM +     ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_NAME +     ">Name</"
        + DataOwnerSingl::XmlKeywords::KEYWORD_NAME +     ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_DATE +     ">2011_12_21_13:23</"
        + DataOwnerSingl::XmlKeywords::KEYWORD_DATE +     ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_GROUPS +   ">one,two</"
        + DataOwnerSingl::XmlKeywords::KEYWORD_GROUPS +   ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_PRIORITY + ">1</"
        + DataOwnerSingl::XmlKeywords::KEYWORD_PRIORITY + ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_TAGS +     ">tag1,tag2</"
        + DataOwnerSingl::XmlKeywords::KEYWORD_TAGS +     ">" + QString(QChar::LineSeparator) + QString("<")
        + DataOwnerSingl::XmlKeywords::KEYWORD_DATA +     "><![CDATA[" + QString(QChar::LineSeparator) + " Example text with html tags" + QString(QChar::LineSeparator) + QString(QChar::LineSeparator) + "      ]]></"
        + DataOwnerSingl::XmlKeywords::KEYWORD_DATA +     ">" + QString(QChar::LineSeparator) + QString("</")
        + DataOwnerSingl::XmlKeywords::KEYWORD_ITEM +     ">" + QString(QChar::LineSeparator) + QString("<");


const QList< QPair<QString, QString> > DataOwnerSingl::XmlKeywords::Keywords = QList< QPair<QString, QString> >()
    << QPair<QString, QString>(XmlKeywords::KEYWORD_NAME,    TxtKeywords::KEYWORD_NAME)
    << QPair<QString, QString>(XmlKeywords::KEYWORD_DATE,    TxtKeywords::KEYWORD_DATE)
    << QPair<QString, QString>(XmlKeywords::KEYWORD_GROUPS,  TxtKeywords::KEYWORD_GROUPS)
    << QPair<QString, QString>(XmlKeywords::KEYWORD_PRIORITY,TxtKeywords::KEYWORD_PRIORITY)
    << QPair<QString, QString>(XmlKeywords::KEYWORD_TAGS,    TxtKeywords::KEYWORD_TAGS)
    << QPair<QString, QString>(XmlKeywords::KEYWORD_DATA,    TxtKeywords::KEYWORD_DATA);

class DataOwnerSingl_private
{
  public:
    DataOwnerSingl_private() {}
    static TimeStampString generateTimeStamp(const QDateTime & Date = QDateTime());
};

TimeStampString DataOwnerSingl_private::generateTimeStamp(const QDateTime & Date)
{
    if (Date.isNull()) //now
        return QDateTime::currentDateTime().toString(DataOwnerSingl::TIMESTAMP_FORMAT);
    else
        return Date.toString(DataOwnerSingl::TIMESTAMP_FORMAT);
    //TimeStampString timeSinceEpochString = ;
    //timeSinceEpochString.setNum(timeSinceEpoch);
    //return timeSinceEpochString;
}
//private HASH


DataOwnerSingl* DataOwnerSingl::m_instancePtr= NULL;
/**
 *  Singleton constructor - defines init state
 */
DataOwnerSingl::DataOwnerSingl()  :
    d_ptr(new DataOwnerSingl_private()),
    sCurrentProject(Settings::General.sCurrentProject),
    currentProjectSettings(NULL)
{
    m_DataFolder = Settings::General.sUserDataDir;
    //this is default fallback
    if (!m_DataFolder.size() || !QFileInfo(m_DataFolder).exists()) {
        m_DataFolder = QCoreApplication::applicationDirPath();
        if (!QFileInfo(m_DataFolder).isWritable()) {
            m_DataFolder = QDir::homePath() + QDir::separator();
            m_DataFolder += "easybrain";
            QDir::current().mkpath(m_DataFolder);
            if (!QFileInfo(m_DataFolder).isWritable())
               qCritical() << m_DataFolder << "Data folder not accassible";
        }
        Settings::General.sUserDataDir = m_DataFolder;
    }
    currentProjectSettings = new ProjectSettings(sCurrentProject, m_DataFolder);
    currentProjectSettings->load();
}

DataOwnerSingl*  DataOwnerSingl::instance()
{
  if (m_instancePtr == NULL)
    m_instancePtr = new DataOwnerSingl();
  return m_instancePtr;
}

QString DataOwnerSingl::dataFolder()
{
    return m_DataFolder;
}

ProjectString DataOwnerSingl::currentProject()
{
   return sCurrentProject;
}

QString DataOwnerSingl::projectFolder(const ProjectString & projectName)
{        
    QString projectFolder = dataFolder() + QDir::separator();
    if (!projectName.size()) {
        Q_ASSERT(currentProject().size());
        projectFolder +=  currentProject();
    }
    else
        projectFolder +=  projectName;

    return projectFolder;
}

QString DataOwnerSingl::newItemFile()
{
  //Q_D(DataOwnerSingl);
  TimeStampString timeStampString = d_ptr->generateTimeStamp();
  //generate new file name
  QString newFileName = projectFolder() + QDir::separator() + WORK_DIR_NAME + QDir::separator();
  newFileName += currentProject() + "_" + d_ptr->generateTimeStamp();
  newFileName += QString(FILE_EXT_EB);

  //check valid filename
  Q_ASSERT_X(newFileName.contains(QRegExp("[\n\r ]")) == false, "DataOwnerSingl::NewItemFilE", "wrong name");

  QFile file(newFileName);
  if (file.exists())
    return QString();

  if (!file.open(QFile::ReadWrite | QFile::Text))
    return QString();

  file.write(DataOwnerSingl::DefaultItemTemplate.toLatin1());
  qDebug() << "file.write(new): "<< newFileName;
  file.close();

  return newFileName;
}

QFileInfoList DataOwnerSingl::projectWorkingFiles(const ProjectString & projectName)
{
    QStringList filters;
    filters << "*" + DataOwnerSingl::FILE_EXT_EB;
    QFileInfoList  projectFolderFileList = QDir(projectFolder(projectName)+ QDir::separator() + WORK_DIR_NAME).entryInfoList( filters, QDir::Files, QDir::Name);
    return projectFolderFileList;
}

QFileInfoList DataOwnerSingl::projectArchiveFiles(const ProjectString & projectName)
{
    QStringList filters;
    filters << "*" + DataOwnerSingl::FILE_EXT_EB;
    QFileInfoList projectFolderFileList = QDir(projectFolder(projectName) + QDir::separator() + ARCHIVE_DIR_NAME).entryInfoList( filters, QDir::Files, QDir::Name);
    return projectFolderFileList;
}

QFileInfoList DataOwnerSingl::projectBackupFiles(const ProjectString & projectName)
{
    QStringList filters;
    filters << "*" + DataOwnerSingl::FILE_EXT_TARBZ2 << "*" + DataOwnerSingl::FILE_EXT_ZIP;
    QFileInfoList projectFolderFileList = QDir(projectFolder(projectName) + QDir::separator() + BACKUP_DIR_NAME).entryInfoList( filters, QDir::Files, QDir::Name);
    return projectFolderFileList;
}

int DataOwnerSingl::projectFilesCount(const ProjectString & projectName )
{
    return projectArchiveFiles(projectName).count() + projectWorkingFiles(projectName).count();
}

TimeStampString DataOwnerSingl::now(QString sFormat)
{
     return QDateTime::currentDateTime().toString(sFormat);
}

void DataOwnerSingl::saveCurrentProjectSettings()
{
    currentProjectSettings->save();
}

QString DataOwnerSingl::XmlKeywords::itemNameFromFile(const QString & sFileName)
{
    QFile file(sFileName);
    QString sNameValue("");

    if (!file.open(QFile::ReadOnly))
        return sNameValue;

    //XML-SAX reading
    qDebug() << "file.read(SAX): "<< sFileName;
    QXmlStreamReader XmlReader;
    XmlReader.setDevice(&file);
    XmlReader.readNext();
    while(!XmlReader.atEnd()) {
        //qDebug() << XmlReader.name() << " ";
        if (XmlReader.name().compare(XmlKeywords::KEYWORD_NAME,Qt::CaseInsensitive) == 0) {
            sNameValue = XmlReader.readElementText();
            break;
       }
       XmlReader.readNext();
    }
    file.close();
    return sNameValue;
}
QString DataOwnerSingl::XmlKeywords::convertKeywordXml2Txt(const QString & xmlTag)
{
    QList< QPair<QString, QString> >::const_iterator i = DataOwnerSingl::XmlKeywords::Keywords.constBegin();
    while (i !=  DataOwnerSingl::XmlKeywords::Keywords.constEnd()) {
        if (xmlTag.compare((*i).first ,Qt::CaseInsensitive) == 0)
            return (*i).second;
        ++i;
    }
    qCritical()<< "xmltag:" << xmlTag << " not recognized";
    Q_ASSERT(0);
    return QString("");
}

