/****************************************************************************
**
** Author: Miroslav Safr <miroslav.safr@gmail.com>
** Web: http://safrm.net/easybrain
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

#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include "defines.h"

class QString;
class GeneralSettings : QSettings {
public:
    explicit GeneralSettings();
    bool load();
    bool save();
public:
  static const QString DEFAULT_sCurrentProject;
  static const bool DEFAULT_bAutoSaveOnExit;
  static const QString DEFAULT_sUserDataDir;
  static const QString DEFAULT_sPackingPassword;
  static const EditorView::Type DEFAULT_eDefaultOpenViewType;

  QString sCurrentProject;
  bool bAutoSaveOnExit;
  QString sUserDataDir;
  QString sPackingPassword;
  EditorView::Type eDefaultOpenViewType;

  //synchronizer
  QString sFtpAddress;
  QString sUserName;
  QString sPassword;
  QString sLastFullSynchTime;
};

class Settings
{
public:
  static Settings* instance();
  void saveSettings();
private:
  explicit Settings();
  static Settings* m_instancePtr;
  Q_DISABLE_COPY(Settings);


   //separated files
 // QSettings * starters;
 // QSettings * network;
public:
 static GeneralSettings General;
};


#endif // SETTINGS_H
