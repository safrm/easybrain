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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "defines.h"

class QAction;
class FileGroupWidget;
class EditorTabWidget;
class MainWindow_private;
class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  MainWindow(const MainWindow& win);  // prevent copy construction
  MainWindow& operator = (const MainWindow&); //prevent equal operator
  void createActions();
  void createMenus();
  void createToolBars();
  void createStatusBar();
  void createDockWindows();

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *projectsMenu;
  QMenu *viewMenuAddedLater;
  QMenu *helpMenu;
  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QToolBar *projectsToolBar;
  QToolBar *viewsToolBar;
  QAction *newItemAct;
  QAction *openAct;
  QAction *openAsSourceAct;
  QAction *saveAct;
  QAction *exportAsAct;
  QAction *emailFileAct;
  QAction *printAct;
  QAction *backupCurrentAct;
  QAction *backupAllAct;
  QAction *synchronizeCurrentAct;
  QAction *synchronizeAllAct;
  QAction *uploadLatestAct;
  QAction *downloadLatestAct;
  QAction *viewDebugWindowAct;
  QAction *showClipboardAct;
  QAction *undoAct;
  QAction *redoAct;
  QAction *cutAct;
  QAction *copyAct;
  QAction *pasteAct;
  QAction *clearAct;
  QAction *selectAllAct;
  QAction *insetShortTimeAct;
  QAction *insetDateAct;
  QAction *aboutAct;
  QAction *quitAct;
  QAction *lockScreenAct;

public:
  explicit MainWindow();
  virtual ~MainWindow();
  //QSize sizeHint() const;
  bool deleteFile(const QString &sFileName);
  static MainWindow *instance();

  EditorTabWidget* editorTab();
  FileGroupWidget* currentProjectFileGroup();


protected slots:
  void slotPlaceToTray();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

  void openAsSource();
  void openAsText();
  void exportAs();
  void showClipboardInToolTip();
  void backupCurrent();
  void backupAll();
  void synchronizeCurrent();
  void synchronizeAll();
  bool downloadLatest();
  bool uploadLatest();
  void about();
  void quit();
  void startUpSlot();
  void viewDebugWindow(bool bToggled);
  void enableToolbars(int);
  void lockScreen();

public slots:
  void emailFile(QString filename = QString());

signals:
  void signalPlaceToTray();

protected:
  virtual void changeEvent(QEvent *ev);
  void closeEvent ( QCloseEvent * event );

private:
  MainWindow_private* d;
};
#define MW MainWindow::instance()
#endif // MAINWINDOW_H
