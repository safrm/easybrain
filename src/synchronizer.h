/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
