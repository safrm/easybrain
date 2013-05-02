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

#include <QCloseEvent>
#include <QComboBox>
#include <QGridLayout>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QProcess>
#include <QTextStream>
#include <QApplication>
#include <QMessageBox>
#include <QDockWidget>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
//locking
#ifdef Q_OS_WIN
#define _WIN32_WINNT 0x0501
#define WINVER 0x0501

#include <windows.h>
#pragma comment( lib, "user32.lib" )
#endif //Q_OS_WIN
#include <stdio.h>

#include "mainwindow.h"
#include "dataowner.h"
//#include "statistics.h"
#include "filegroupwidget.h"
#include "settings.h"
#include "debug.h"
//#include "synchronizer.h"
#include "editortabwidget.h"
#include "ebtextedit.h"
//#include "aboutdialog.h"

const QStringList DLG_OPEN_FILTERS = (QStringList()
                                      << QString("Easybrain files (*.eb)") // + DataOwnerSingl::FILE_EXT_EB + QString(")")
                                      << QString("Text files (*.txt)")     // + DataOwnerSingl::FILE_EXT_TXT + QString(")")
                                      << "Any files (*)");
MainWindow * g_pMainGuiWindow =NULL;


class MainWindow_private : public QObject
{
  public:
    explicit MainWindow_private(QObject* parent) :
            QObject(parent),
            m_pTrayIcon(0),
            m_pWhatComboBox(0),
            m_pOpenFileDialog(0),
            m_pExportAsFileDialog(0),
            bCanQuit(false),
//            synchronizer(NULL),
            tabWidget(NULL),
            currentProjectFileGroup(NULL)
//            aboutDialog(NULL)
    {};
    QFileDialog* openFileDialog();
    QFileDialog* exportAsFileDialog();
    QSystemTrayIcon* m_pTrayIcon;
    QComboBox* m_pWhatComboBox;
    QFileDialog* m_pOpenFileDialog;
    QFileDialog* m_pExportAsFileDialog;
    bool bCanQuit;
//    Synchronizer* synchronizer;
    EditorTabWidget* tabWidget;
    FileGroupWidget* currentProjectFileGroup;
//    AboutDialog* aboutDialog;
};
QFileDialog* MainWindow_private::openFileDialog()
{
  if (!m_pOpenFileDialog) {
        m_pOpenFileDialog = new QFileDialog;
        m_pOpenFileDialog->setModal(true);
        m_pOpenFileDialog->setWindowIcon(QIcon(":/images/open.svg"));
        m_pOpenFileDialog->setAcceptMode(QFileDialog::AcceptOpen);

        m_pOpenFileDialog->setNameFilters(DLG_OPEN_FILTERS);
        m_pOpenFileDialog->selectNameFilter(DataOwnerSingl::FILE_EXT_EB);

        m_pOpenFileDialog->setFileMode(QFileDialog::ExistingFiles); // AnyFile
        m_pOpenFileDialog->setViewMode(QFileDialog::Detail); // List
      }
      return m_pOpenFileDialog;
}


QFileDialog* MainWindow_private::exportAsFileDialog()
{
  if (!m_pExportAsFileDialog) {
    m_pExportAsFileDialog = new QFileDialog;
    m_pExportAsFileDialog->setModal(true);
    m_pExportAsFileDialog->setWindowIcon(QIcon(":/images/export_as.svg"));
    m_pExportAsFileDialog->setAcceptMode(QFileDialog::AcceptSave);
    m_pExportAsFileDialog->selectNameFilter(DataOwnerSingl::FILE_EXT_EB);

    m_pExportAsFileDialog->setNameFilters(DLG_OPEN_FILTERS);
    m_pExportAsFileDialog->setFileMode(QFileDialog::AnyFile); // ExistingFile
    m_pExportAsFileDialog->setViewMode(QFileDialog::Detail); // List
  }
  return m_pExportAsFileDialog;
}
MainWindow* MainWindow::instance()
{
    return  g_pMainGuiWindow;
}

MainWindow::MainWindow() :
  d(new MainWindow_private(this))
{
    g_pMainGuiWindow = this;
    Settings::instance();
//debug stuff

    setWindowIcon(QIcon(":/images/tray_icon.svg"));
    d->m_pTrayIcon = new QSystemTrayIcon(QIcon(":/images/tray_icon.svg"),this);
    connect(this, SIGNAL(signalPlaceToTray()),this, SLOT(slotPlaceToTray()),Qt::QueuedConnection);
    connect(d->m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    d->m_pTrayIcon->show();

//    d->synchronizer = new Synchronizer(this);

    createDockWindows();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    //EditorTab -> ProjectGroup -> workingList->row (not so stright forward)
    connect(editorTab(), SIGNAL(refreshFileRow(const QString & , const QString & )),  currentProjectFileGroup(), SIGNAL(refreshFileRow(const QString & , const QString & )));

    //open last or make new
    QTimer::singleShot(100, this, SLOT(startUpSlot()));
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
              emit signalPlaceToTray();
              event->ignore();
              return;
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::slotPlaceToTray()
{
    //d->m_pTrayIcon->show();
    hide();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
            //d->m_pTrayIcon->hide();
            //this way it shows on top
            //setWindowFlags(Qt::WindowStaysOnTopHint);
            //show();
            //setWindowFlags(0);
            if (isVisible())
              hide();
            else
              showNormal();
            break;
    case QSystemTrayIcon::MiddleClick:
        // Lock the workstation.
#ifdef Q_OS_WIN
            if ( !LockWorkStation() )
                qCritical("LockWorkStation failed with %ld\n", GetLastError());
#endif //Q_OS_WIN
#ifdef Q_OS_UNIX
            {
                QProcess locker;
                locker.start("gnome-screensaver-command", QStringList() << "-l");
                locker.waitForStarted();
                locker.waitForFinished();
            }
#endif //Q_OS_UNIX
        break;
    default:
            ;
    }
}
/*QSize MainWindow::sizeHint() const
{
    return QSize(1400,1024);
}
*/
MainWindow::~MainWindow()
{
    delete d;
}
void MainWindow::createActions()
{
    newItemAct = new QAction(QIcon(":/images/new.svg"), tr("&New Item"), this);
    newItemAct->setShortcuts(QKeySequence::New);
    newItemAct->setStatusTip(tr("Create a new item"));
    connect(newItemAct, SIGNAL(triggered()), editorTab(), SLOT(addNewItem()));

    openAct = new QAction(QIcon(":/images/open.svg"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open item"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openAsText()));


    openAsSourceAct = new QAction(QIcon(":/images/open.svg"), tr("&Open as source..."), this);
    //openAsTextAct->setShortcuts(QKeySequence::Open);
    openAsSourceAct->setStatusTip(tr("Open item as Source"));
    connect(openAsSourceAct, SIGNAL(triggered()), this, SLOT(openAsSource()));

    saveAct = new QAction(QIcon(":/images/save.svg"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save item"));
    connect(saveAct, SIGNAL(triggered()), editorTab(), SLOT(saveCurrentItem()));

    exportAsAct = new QAction(QIcon(":/images/export_as.svg"), tr("&Export As..."), this);
    exportAsAct->setShortcuts(QKeySequence::SaveAs);
    exportAsAct->setStatusTip(tr("Export As new item"));
    connect(exportAsAct, SIGNAL(triggered()), this, SLOT(exportAs()));

    emailFileAct = new QAction(QIcon(":/images/mail_file.svg"), tr("&Send file by email."), this);
    //mailFileAct->setShortcuts(QKeySequence::SaveAs);
    emailFileAct->setStatusTip(tr("Send file by email"));
    connect(emailFileAct, SIGNAL(triggered()), this, SLOT(emailFile()));

    printAct = new QAction(QIcon(":/images/print.svg"), tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the current item"));
    connect(printAct, SIGNAL(triggered()), editorTab(), SLOT(printCurrentItem()));


    showClipboardAct = new QAction(QIcon(":/images/show_clipboard.svg"), tr("&Show clipboard"), this);
    //showClipboardAct->setShortcuts(QKeySequence::Undo);
    showClipboardAct->setStatusTip(tr("Show content of clipboard"));
    connect(showClipboardAct, SIGNAL(hovered()), this, SLOT(showClipboardInToolTip()));
    connect(showClipboardAct, SIGNAL(triggered()), this, SLOT(showClipboardInToolTip()));

    undoAct = new QAction(QIcon(":/images/undo.svg"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), editorTab(), SLOT(undo()));

    redoAct = new QAction(QIcon(":/images/redo.svg"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last editing action"));
    connect(redoAct, SIGNAL(triggered()), editorTab(), SLOT(redo()));

    cutAct = new QAction(QIcon(":/images/cut.svg"), tr("&Cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut selected text to clipboard"));
    connect(cutAct, SIGNAL(triggered()), editorTab(), SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.svg"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy selected text to clipboard"));
    connect(copyAct, SIGNAL(triggered()), editorTab(), SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.svg"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste text from clipboard"));
    connect(pasteAct, SIGNAL(triggered()), editorTab(), SLOT(paste()));

    clearAct = new QAction(QIcon(":/images/clear.svg"), tr("&Clear"), this);
    clearAct->setShortcuts(QKeySequence::Delete);
    clearAct->setStatusTip(tr("Clear selected text"));
    connect(clearAct, SIGNAL(triggered()), editorTab(), SLOT(clear()));

    selectAllAct = new QAction(QIcon(":/images/select_all.svg"), tr("Select &All"), this);
    selectAllAct->setShortcuts(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select all text"));
    connect(selectAllAct, SIGNAL(triggered()), editorTab(), SLOT(selectAll()));

    insetShortTimeAct = new QAction(QIcon(":/images/insert_time.svg"), tr("&Insert short time"), this);
    //insetShortTimeAct->setShortcuts(QKeySequence::Undo);
    insetShortTimeAct->setStatusTip(tr("Insert short time"));
    connect(insetShortTimeAct, SIGNAL(triggered()), editorTab(), SLOT(insetShortTime()));


    insetDateAct = new QAction(QIcon(":/images/insert_date.svg"), tr("&Insert date"), this);
    insetDateAct->setStatusTip(tr("Insert date"));
    connect(insetDateAct, SIGNAL(triggered()), editorTab(), SLOT(insetDate()));


    backupCurrentAct = new QAction(QIcon(":/images/backup.svg"), tr("&Backup current"), this);
    backupCurrentAct->setStatusTip(tr("Backup current"));
    connect(backupCurrentAct, SIGNAL(triggered()), this, SLOT(backupCurrent()));

    backupAllAct = new QAction(QIcon(":/images/backup_all.svg"), tr("&Backup all"), this);
    backupAllAct->setStatusTip(tr("Backup all"));
    connect(backupAllAct, SIGNAL(triggered()), this, SLOT(backupAll()));

/*
    synchronizeCurrentAct = new QAction(QIcon(":/images/synch.svg"), tr("&Synchronize current"), this);
    //synchronizeCurentAct->setShortcuts(QKeySequence::Undo);
    synchronizeCurrentAct->setStatusTip(tr("Synchronize current"));
    connect(synchronizeCurrentAct, SIGNAL(triggered()), this, SLOT(synchronizeCurrent()));

    synchronizeAllAct = new QAction(QIcon(":/images/synch_all.svg"), tr("&Synchronize all"), this);
    //synchronizeAllAct->setShortcuts(QKeySequence::Undo);
    synchronizeAllAct->setStatusTip(tr("Synchronize all"));
    connect(synchronizeAllAct, SIGNAL(triggered()), this, SLOT(synchronizeAll()));
*/
    uploadLatestAct = new QAction(QIcon(":/images/synch.svg"), tr("&Upload latest"), this);
    uploadLatestAct->setStatusTip(tr("Synchronize current"));
    connect(uploadLatestAct, SIGNAL(triggered()), this, SLOT(uploadLatest()));

    downloadLatestAct = new QAction(QIcon(":/images/download_latest.svg"), tr("&Download latest"), this);
    downloadLatestAct->setStatusTip(tr("Download current"));
    connect(downloadLatestAct, SIGNAL(triggered()), this, SLOT(downloadLatest()));


    quitAct = new QAction(QIcon(":/images/quit.svg"), tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));

    aboutAct = new QAction(QIcon(":/images/about.svg"), tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newItemAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(openAsSourceAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exportAsAct);
    fileMenu->addAction(emailFileAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(showClipboardAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(clearAct);
    editMenu->addAction(selectAllAct);
    editMenu->addSeparator();
    editMenu->addAction(insetShortTimeAct);
    editMenu->addAction(insetDateAct);
    connect(editorTab(),SIGNAL(currentChanged(int)),this,SLOT(enableToolbars(int)));

    //viewMenu = menuBar()->addMenu(tr("&View"));
    menuBar()->addMenu(viewMenuAddedLater);

    projectsMenu = menuBar()->addMenu(tr("&Projects"));
    projectsMenu->addAction(backupCurrentAct);
    projectsMenu->addAction(backupAllAct);
    projectsMenu->addSeparator();
    projectsMenu->addAction(uploadLatestAct);
    projectsMenu->addAction(downloadLatestAct);
    projectsMenu->addSeparator();
    projectsMenu->addAction(synchronizeCurrentAct);
    projectsMenu->addAction(synchronizeAllAct);


    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}
void MainWindow::enableToolbars(int index)
{
    //disable some features when there is not open any item
    if (index == -1 )  {
      saveAct->setDisabled(true);
      exportAsAct->setDisabled(true);
      emailFileAct->setDisabled(true);
      printAct->setDisabled(true);
      undoAct->setDisabled(true);
      redoAct->setDisabled(true);
      cutAct->setDisabled(true);
      copyAct->setDisabled(true);
      pasteAct->setDisabled(true);
      clearAct->setDisabled(true);
      selectAllAct->setDisabled(true);
      insetShortTimeAct->setDisabled(true);
      insetDateAct->setDisabled(true);
    }
    else {
        saveAct->setEnabled(true);
        exportAsAct->setEnabled(true);
        emailFileAct->setEnabled(true);
        printAct->setEnabled(true);
        undoAct->setEnabled(true);
        redoAct->setEnabled(true);
        cutAct->setEnabled(true);
        copyAct->setEnabled(true);
        pasteAct->setEnabled(true);
        clearAct->setEnabled(true);
        selectAllAct->setEnabled(true);
        insetShortTimeAct->setEnabled(true);
        insetDateAct->setEnabled(true);
    }
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newItemAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(openAsSourceAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(exportAsAct);
    fileToolBar->addAction(emailFileAct);
    fileToolBar->addAction(printAct);
#ifdef DEBUG
    fileToolBar->addSeparator();
    fileToolBar->addAction(quitAct);
#endif //DEBUG

    projectsToolBar = addToolBar(tr("Projects"));
    projectsToolBar->addAction(backupCurrentAct);
    projectsToolBar->addAction(backupAllAct);
    projectsToolBar->addSeparator();
    projectsToolBar->addAction(uploadLatestAct);
    projectsToolBar->addAction(downloadLatestAct);
    projectsToolBar->addSeparator();
    projectsToolBar->addAction(synchronizeCurrentAct);
    projectsToolBar->addAction(synchronizeAllAct);

    //lower toolbars
    editToolBar = new QToolBar(tr("Edit"),this);
    editToolBar->addAction(undoAct);
    editToolBar->addAction(showClipboardAct);
    editToolBar->addAction(insetShortTimeAct);
    editToolBar->addAction(insetDateAct);
    addToolBar(Qt::BottomToolBarArea, editToolBar);

    viewsToolBar = new QToolBar(tr("Views"),this);
    viewsToolBar->addActions(editorTab()->changeViewActionGroup()->actions());
    addToolBar(Qt::BottomToolBarArea, viewsToolBar);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    viewMenuAddedLater = new QMenu(tr("&View"));
    QDockWidget *dock = new QDockWidget(tr("Texts"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    d->tabWidget = new EditorTabWidget(dock);
    dock->setWidget(d->tabWidget);
    setCentralWidget(dock);

    viewMenuAddedLater->addAction(dock->toggleViewAction());

    /*dock = new StatisticsDock(this);
    dock->setVisible(false); //default not show
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenuAddedLater->addAction(dock->toggleViewAction());
*/
    dock = new QDockWidget(DO->currentProject(), this);
    d->currentProjectFileGroup = new FileGroupWidget(DO->currentProject(),dock);
    dock->setWidget(currentProjectFileGroup());
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    viewMenuAddedLater->addAction(dock->toggleViewAction());


    //last debug
    viewMenuAddedLater->addSeparator();
    viewDebugWindowAct = new QAction(QIcon(":/images/debug.svg"), tr("&Debug window"), this);
    //viewDebugWindowAct->setShortcuts(QKeySequence::New);
    viewDebugWindowAct->setStatusTip(tr("ShowDebugWindow"));
    viewDebugWindowAct->setCheckable(true);
#ifdef DEBUG
    //viewDebugWindowAct->setChecked(true);
#endif //DEBUG
    connect(viewDebugWindowAct, SIGNAL(toggled ( bool )), this, SLOT(viewDebugWindow(bool )));
    viewMenuAddedLater->addAction(viewDebugWindowAct);
}

void MainWindow::openAsSource()
{
    d->openFileDialog()->setDirectory(DO->projectFolder());
    if (!d->openFileDialog()->exec())
        return;

    QStringList fileNames = d->openFileDialog()->selectedFiles();
    if (fileNames.size()) {
        //d->m_FileDialogsDirectory = openFileDialog()->directory();
        //d->namefilter = openFileDialog()->selectedNameFilter();
        foreach (const QString& sFileName,fileNames) {
            editorTab()->addOpenItem(sFileName, EditorView::XmlSource);
      }
    }
}
void MainWindow::openAsText()
{
    d->openFileDialog()->setDirectory(DO->projectFolder());
    if (!d->openFileDialog()->exec())
        return;

    QStringList fileNames = d->openFileDialog()->selectedFiles();
    if (fileNames.size()) {
        //d->m_FileDialogsDirectory = openFileDialog()->directory();
        //d->namefilter = openFileDialog()->selectedNameFilter();
        foreach (const QString& sFileName,fileNames) {
            editorTab()->addOpenItem(sFileName, EditorView::Text);
      }
    }
}
bool MainWindow::deleteFile(const QString &sFileName)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Delete file"),
                                      tr("Do you want to <b>delete</b> %1 ?").arg(qPrintable(sFileName)),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

    if (reply != QMessageBox::Yes)
        return false;

    //only if it is open
    if (editorTab()->isFileOpen(sFileName))
        editorTab()->closeTab(sFileName);


    QFile::remove(sFileName);

    statusBar()->showMessage(sFileName + tr(" deleted"));
    return true;
}

void MainWindow::exportAs()
{
    // restore status
    d->exportAsFileDialog()->setDirectory(DO->projectFolder());
    //if (!d->m_pMainTextEdit->toolTip().isEmpty())
    QString sFileName = editorTab()->currentTabFileName();
    if (!sFileName.isEmpty())
        d->exportAsFileDialog()->selectFile(sFileName);

    if (d->exportAsFileDialog()->exec()) {
        QStringList fileNames = d->exportAsFileDialog()->selectedFiles();
        //TODO check if the filename contains full path; if not, complete it
        if (!fileNames[0].isEmpty()) {
            editorTab()->saveAsCurrentItem(fileNames[0]);
        }
    }
}

void MainWindow::about()
{
/*    if (!d->aboutDialog) {
        d->aboutDialog = new AboutDialog(this);
    }
    d->aboutDialog->show();
    */
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!d->bCanQuit) {
    event->ignore();
    if (isVisible())
      hide();
 }
 else {
    QMainWindow::closeEvent(event);
  }
}

void MainWindow::quit()
{
    if (Settings::General.bAutoSaveOnExit) {
        if (!editorTab()->saveAll()) {
            QMessageBox::StandardButton reply = QMessageBox::warning(this, tr("Closing Easibrain"),
                                              tr("Auto saving all files failed. Do you want to anyway <b>quit</b> application?"),
                                              QMessageBox::Yes | QMessageBox::No);
            if (reply != QMessageBox::Yes)
               return;
        }
    }
    Settings::instance()->saveSettings();
    DO->saveCurrentProjectSettings();
    d->bCanQuit = true;
    emit close();
}

void MainWindow::synchronizeCurrent()
{
    if(!downloadLatest())
        return;

    //compration, correction, delete temporary files
    uploadLatest();
}

void MainWindow::synchronizeAll()
{
}

bool MainWindow::uploadLatest()
{
 /*   QString sFilename = d->synchronizer->packProject(true);
    if (sFilename.size()) {
        if (!d->synchronizer->uploadFile(sFilename)) {
                QMessageBox::warning(this, tr("EB"), tr("Cannot upload file %1.").arg(sFilename));
                return false;
        }
        else
            return true;
    }
    */
    return false;
}

bool MainWindow::downloadLatest()
{
 /*   if (!d->synchronizer->downloadFile()) {
       QMessageBox::warning(this, tr("EB"), tr("Cannot download latest file."));
       return false;
    }
    */
    return true;
}

void MainWindow::backupCurrent()
{
   /* QString sFilename = d->synchronizer->backupProject();
    if (sFilename.size()) {
        currentProjectFileGroup()->addNewBackup(sFilename);
    }*/
}
void MainWindow::backupAll()
{
}

void MainWindow::startUpSlot()
{
    QFileInfoList workFiles = DO->projectWorkingFiles();
    if (workFiles.count()) {
        editorTab()->addOpenItem(workFiles.first().absoluteFilePath());
    }
    else
         editorTab()->addNewItem();
}

void MainWindow::viewDebugWindow(bool bToggled)
{

}
void MainWindow::emailFile(QString fileName)
{
    if (!fileName.size())
        fileName = editorTab()->currentTabFileName();
    Q_ASSERT(fileName.size());

    if (!fileName.isEmpty()) {
        QString sMailTo = QString("mailto:?subject=test &attach=") ;
        sMailTo += fileName;
        //sMailTo += QString("\"") ; //test on win
        QDesktopServices::openUrl(QUrl(sMailTo));
    }
}

EditorTabWidget* MainWindow::editorTab()
{
    Q_ASSERT(d->tabWidget);
    return d->tabWidget;
}

FileGroupWidget* MainWindow::currentProjectFileGroup()
{
    Q_ASSERT(d->currentProjectFileGroup);
    return d->currentProjectFileGroup;
}

void MainWindow::showClipboardInToolTip()
{
    if (QApplication::clipboard()->text() != showClipboardAct->toolTip())
        showClipboardAct->setToolTip(QApplication::clipboard()->text());
}

