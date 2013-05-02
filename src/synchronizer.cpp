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
#include <QProcess>
#include <QDir>
#include <QMessageBox>

#include <QFtp>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStatusBar>

#include "synchronizer.h"
#include "dataowner.h"
#include "settings.h"
#include "mainwindow.h"

Synchronizer::Synchronizer(QObject* parent ) :
    QObject(parent),
    m_ftp(NULL),
    m_TransferFile(),
    m_RemoteDir()
{

}

Synchronizer::TransferFile::TransferFile() :
    LocalFullName(""),
    RemoteName(""),
    File(NULL),
    Type(Empty)
{
}

Synchronizer::CurrentRemoteDir::CurrentRemoteDir() :
    RelativePath("")
{
    FileList = new QStringList();
}

void Synchronizer::CurrentRemoteDir::clear()
{
    RelativePath = "";
    FileList->clear();
}
void Synchronizer::CurrentRemoteDir::changeDir(QString newRelativePath)
{
    if(newRelativePath.startsWith(QDir::separator()))
        RelativePath = newRelativePath;
    else
        RelativePath +=  QDir::separator() + newRelativePath;
    FileList->clear();
}

QString Synchronizer::packProject(bool bUsePassword, const ProjectString & projectName)
{
    bool bError(false);
    //QString sFolder = DO->projectFolder(projectName) + QDir::separator() + DataOwnerSingl::WORK_DIR_NAME + QDir::separator();
    QString sFolder("");
    if (projectName.size())
         sFolder += projectName;
    else
        sFolder += DO->currentProject();
    sFolder += QDir::separator() +  DataOwnerSingl::WORK_DIR_NAME + QDir::separator();

    QString sFileName(projectName);
    if (!sFileName.size())
        sFileName = DO->currentProject();
    sFileName += "_" + DataOwnerSingl::WORK_DIR_NAME + "_" + DO->now() + DataOwnerSingl::FILE_EXT_ZIP;
    QString sAbsolutFileName = DO->projectFolder(projectName)  + QDir::separator() + DataOwnerSingl::BACKUP_DIR_NAME +  QDir::separator() + sFileName;
    qWarning()<< "packing " <<  sFolder << " to " << sAbsolutFileName;

    //check valid filename
    Q_ASSERT_X(sAbsolutFileName.contains(QRegExp("[\n\r ]")) == false, "Synchronizer::PackProject", "wrong name");     

    QProcess proc;
//    const QString program = "/bin/tar";
//    const QStringList arguments = QStringList() << QLatin1String("cvjf") <<  sAbsolutFileName << sFolder;
    //todo zip
    //zip -P password -r ZZZZ.zip ZZZZ.TXT
    const QString program = "/usr/bin/zip";
    QStringList arguments = QStringList();
    if (bUsePassword)
        arguments << "-P" << Settings::General.sPackingPassword;

    arguments << QLatin1String("-r") <<  sAbsolutFileName << sFolder;


    proc.setWorkingDirectory(DO->dataFolder());
    proc.start(program, arguments);
    proc.waitForStarted();
    proc.waitForFinished();
    if (proc.exitStatus() != QProcess::NormalExit) {
        qCritical() << "Process exit status error ";
        bError = true;
    }
    if (proc.exitCode() != 0) {
        qCritical() << "Process exit code err ";
        bError = true;
    }
    if (!bError)
      return sAbsolutFileName;

    if (QFileInfo(sAbsolutFileName).exists())
        QFile(sAbsolutFileName).remove();
    return "";

    //TODO add to  fileexplorer
}

QString Synchronizer::backupProject(const ProjectString & projectName)
{
    QString sBackupFile = packProject(false, projectName);
    //if (!sBackupFile.size())
    //todo statistics
    return sBackupFile;
}

bool Synchronizer::uploadFile(const QString & sBackupFile)
{
    if (m_TransferFile.Type!=  TransferFile::Empty) {
        qCritical() << "FTP: unfinished operation" << m_TransferFile.Type << " "
                    << m_TransferFile.LocalFullName << " " << m_TransferFile.RemoteName;
        return false;
    }
    m_TransferFile.Type = TransferFile::Upload;
    m_TransferFile.LocalFullName = sBackupFile;
    m_TransferFile.RemoteName = QFileInfo(sBackupFile).fileName();

    QString sServer = Settings::General.sFtpAddress;
    QString sUserName =  Settings::General.sUserName;
    QString sPassword = Settings::General.sPassword;


    if (sServer.contains("ftp://"))
        sServer = sServer.right(sServer.size() - QString("ftp://").length());
    ftp()->connectToHost(sServer, 21);
    ftp()->login(sUserName, sPassword);

    m_RemoteDir.clear();
    ftp()->list();

    qWarning()<< "uploading " <<  m_TransferFile.LocalFullName << " to " <<
        sServer + QDir::separator() +  DO->currentProject() + QDir::separator() + m_TransferFile.RemoteName;

    return true;
}

bool Synchronizer::downloadFile(const QString & sDownloadedFile)
{

    if (m_TransferFile.Type!=  TransferFile::Empty) {
        qCritical() << "FTP: unfinished operation" << m_TransferFile.Type << " "
                    << m_TransferFile.LocalFullName << " " << m_TransferFile.RemoteName;
        return false;
    }
    m_TransferFile.Type = TransferFile::Download;
    m_TransferFile.LocalFullName = "";
    m_TransferFile.RemoteName = "";

    QString sServer = "ftp://easybrain-data.safrm.net";
    QString sUserName = "ftp73489";
    QString sPassword = "deset";


    if (sServer.contains("ftp://"))
        sServer = sServer.right(sServer.size() - QString("ftp://").length());
    ftp()->connectToHost(sServer, 21);
    ftp()->login(sUserName, sPassword);

    m_RemoteDir.clear();
    ftp()->list();
/*
    qWarning()<< "downloading " <<  m_TransferFile.Name << " to " <<
        sServer + QDir::separator() +  DO->currentProject() + QDir::separator() + QFileInfo(m_TransferFile.Name).fileName();
*/
    return true;
}

QFtp *Synchronizer::ftp()
{
    if (!m_ftp) {
        m_ftp = new QFtp(this);
        connect(m_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
        connect(m_ftp, SIGNAL(dataTransferProgress(qint64,qint64)),this, SLOT(updateDataTransferProgress(qint64,qint64)));
        connect(m_ftp, SIGNAL(listInfo( const QUrlInfo &)), this, SLOT(ftpListInfo( const QUrlInfo &)));
    }
    return m_ftp;
}

void Synchronizer::updateDataTransferProgress(qint64 done,qint64 total)
{
    QString sMessage = QString("updateDataTransferProgress: done:%1 total:%2").arg(done).arg(total);
    qWarning()<< "FTP: updateDataTransferProgress:" << sMessage;
}

void Synchronizer::ftpCommandFinished(int id ,bool error)
{
    QFtp::Command currentCommand = ftp()->currentCommand();
    QString sError("");
    if (error)
        sError = " error " + ftp()->errorString();
    qWarning()<< "FTP: ftpCommandFinished: id:" <<  id<<  ftpCommandName(currentCommand)  << qPrintable(sError);

    if (currentCommand == QFtp::List) {
        qWarning()<< "FTP: QFtp::List " << m_RemoteDir.RelativePath << ": " << qPrintable(m_RemoteDir.FileList->join(" "));

        //the folder does not exists or we go there
        if(m_RemoteDir.RelativePath.isEmpty())
        {
            if (!m_RemoteDir.FileList->contains(DO->currentProject())) {
                qWarning()<< "FTP: creating directory:" <<  DO->currentProject();
                ftp()->mkdir(DO->currentProject());
            }
            m_RemoteDir.changeDir(DO->currentProject());
            ftp()->cd(DO->currentProject());

            //start upload
            if (m_TransferFile.Type == TransferFile::Upload) {
                //maybe check if it exists
                m_TransferFile.File = new QFile(m_TransferFile.LocalFullName);
                if (!m_TransferFile.File->exists() || !m_TransferFile.File->open(QIODevice::ReadOnly)) {
                    qWarning()<< "FTP:  uploading " <<  m_TransferFile.LocalFullName << "failed ";
                    m_TransferFile.LocalFullName = "";
                    m_TransferFile.RemoteName = "";
                    delete m_TransferFile.File;
                    m_TransferFile.File = NULL;
                    ftp()->close();
                    m_TransferFile.Type = TransferFile::Empty;
                } else {
                    ftp()->put(m_TransferFile.File, m_TransferFile.RemoteName);
                }
            } else if (m_TransferFile.Type == TransferFile::Download )
                ftp()->list(); //request file list
        }
        else { //we assume just 2 states for now
            if (m_TransferFile.Type == TransferFile::Download) {
                if (m_RemoteDir.FileList->isEmpty())
                    qWarning()<< "FTP : no files in remote side";
                else {
                    m_RemoteDir.FileList->removeOne(".");
                    m_RemoteDir.FileList->removeOne("..");
                    m_RemoteDir.FileList->sort();
                    qWarning()<< "FTP: QFtp::List " << m_RemoteDir.RelativePath << ": " << qPrintable(m_RemoteDir.FileList->join(" "));
                    //get newest one
                    m_TransferFile.RemoteName = m_RemoteDir.FileList->last();
                    m_TransferFile.LocalFullName = DO->projectFolder() + QDir::separator() + DataOwnerSingl::TEMP_DIR_NAME
                            +  QDir::separator() + m_TransferFile.RemoteName;
                    m_TransferFile.File = new QFile(m_TransferFile.LocalFullName);
                    if (m_TransferFile.File->exists())
                        qWarning()<< "FTP:  download: " <<  m_TransferFile.LocalFullName << "file exists - rewritting";
                    if(!m_TransferFile.File->open(QIODevice::WriteOnly)) {
                        qCritical()<< "FTP:  download: " <<  m_TransferFile.LocalFullName << "filed";
                        m_TransferFile.LocalFullName = "";
                        m_TransferFile.RemoteName = "";
                        delete m_TransferFile.File;
                        m_TransferFile.File = NULL;
                        ftp()->close();
                        m_TransferFile.Type = TransferFile::Empty;
                    }
                    else
                      ftp()->get(m_TransferFile.RemoteName,m_TransferFile.File);
                }
            }
        }

    }
    else if (currentCommand == QFtp::Put) {
        //uploaded ok
        if (!error) {
            QString sMessage = QString ("FTP: ") + m_TransferFile.LocalFullName + " uploaded ok";
            qWarning()<< sMessage;
            MW->statusBar()->showMessage(sMessage);
        }
        m_TransferFile.File->close();
        m_TransferFile.LocalFullName = "";
        m_TransferFile.RemoteName = "";
        delete m_TransferFile.File;
        m_TransferFile.File = NULL;
        ftp()->close();
        m_TransferFile.Type = TransferFile::Empty;
    }
    else if (currentCommand == QFtp::Get) {
        //downloaded ok
        if (!error) {
            QString sMessage = QString ("FTP: ") + m_TransferFile.LocalFullName + QString(" downloaded ok ")
            + QString::number(m_TransferFile.File->size()) + QString("b");
            qWarning()<< sMessage;
            MW->statusBar()->showMessage(sMessage);
        }
        m_TransferFile.File->close();
        m_TransferFile.LocalFullName = "";
        m_TransferFile.RemoteName = "";
        delete m_TransferFile.File;
        m_TransferFile.File = NULL;
        ftp()->close();
        m_TransferFile.Type = TransferFile::Empty;
    }
}

void Synchronizer::ftpListInfo(const QUrlInfo & list)
{
    QString sUrl = list.name();
    m_RemoteDir.FileList->append(list.name());
}

QString Synchronizer::ftpCommandName(int iCommand)
{
    switch(iCommand) {
    case QFtp::None:	        return "0 - No command ";
    case QFtp::SetTransferMode: return "1 - set the transfer mode.";
    case QFtp::SetProxy:	    return "2 - switch proxying on or off.";
    case QFtp::ConnectToHost:   return "3 - connectToHost()";
    case QFtp::Login:           return "4 - login()";
    case QFtp::Close:	        return "5 - close()";
    case QFtp ::List:           return "6 - list()";
    case QFtp::Cd:              return "7 - cd()";
    case QFtp::Get:             return "8 - get()";
    case QFtp::Put:             return "9 - put()";
    case QFtp::Remove:          return "10 - remove()";
    case QFtp::Mkdir:           return "11 - mkdir()";
    case QFtp::Rmdir:           return "12 - rmdir()";
    case QFtp::Rename:          return "13 - rename()";
    case QFtp::RawCommand:      return "14 - rawCommand()";
    default :
        Q_ASSERT(0);
    }
    Q_ASSERT(0);
    return "unknown";
}
