/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
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
