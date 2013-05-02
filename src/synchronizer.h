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

#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H
#include <QObject>
#include <QUrlInfo>

#include "defines.h"

class QFtp;
class QFile;



class Synchronizer  : public QObject {
      Q_OBJECT
private slots:
    void updateDataTransferProgress(qint64,qint64);
    void ftpCommandFinished(int,bool);
    void ftpListInfo(const QUrlInfo & list);

public:
    explicit Synchronizer(QObject* parent);
    QString packProject(bool bUsePassword, const ProjectString & projectName = QString(""));
    QString backupProject(const ProjectString & projectName = QString(""));
    bool uploadFile(const QString & sBackupFile);
    bool downloadFile(const QString & sDownloadedFile = QString(""));
    QFtp *ftp();
    static QString ftpCommandName(int iCommand);

private:
     QFtp *m_ftp;
     class TransferFile {
        public:
         explicit TransferFile();
         QString LocalFullName;
         QString RemoteName;
         QFile* File;
         enum {Empty, Upload, Download} Type;
     } m_TransferFile;
     class CurrentRemoteDir {
     public:
        explicit CurrentRemoteDir();
        void clear();
        void changeDir(QString RelativePath);

        QString RelativePath;
        QStringList* FileList;
     } m_RemoteDir;
};

#endif // SYNCHRONIZER_H
