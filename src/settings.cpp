/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#include <QDebug>
#include <QSettings>
#include <QRegExp>
#include "settings.h"
#include "editortabwidget.h"


Settings* Settings::m_instancePtr = NULL;
GeneralSettings Settings::General;

const QString GeneralSettings::DEFAULT_sCurrentProject("test");
const bool GeneralSettings::DEFAULT_bAutoSaveOnExit(true);
const QString GeneralSettings::DEFAULT_sUserDataDir("~/easybrain");
const QString GeneralSettings::DEFAULT_sPackingPassword("easybrain123");
const EditorView::Type GeneralSettings::DEFAULT_eDefaultOpenViewType(EditorView::XmlSource);

GeneralSettings::GeneralSettings() :
        QSettings("easybrain", "general")
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
}

bool GeneralSettings::load()
{
    //general settings and their default values
    //~/.config/easybrain/general.conf
    qDebug() << "Loading " << qPrintable(fileName());
    sCurrentProject = value("CurrentProject", DEFAULT_sCurrentProject).toString();
    //check valid projectname
    Q_ASSERT_X(sCurrentProject.contains(QRegExp("[\n\r ]")) == false, "GeneralSettings::Load", "wrong project name");

    qDebug() << " general/CurrentProject: " << qPrintable(sCurrentProject);
    bAutoSaveOnExit = value("AutoSaveOnExit", DEFAULT_bAutoSaveOnExit).toBool();
    qDebug() << " general/bAutoSaveOnExit: " << bAutoSaveOnExit;
    sUserDataDir = value("UserDataDir", DEFAULT_sUserDataDir).toString();
    qDebug() << " general/UserDataDir: " << qPrintable(sUserDataDir);
    sPackingPassword = value("PackingPassword", DEFAULT_sPackingPassword).toString();

    eDefaultOpenViewType = EditorView::String2Enum(
                    value("DefaultOpenViewType", EditorView::Enum2String(DEFAULT_eDefaultOpenViewType)).toString());
    qDebug() << " general/DefaultOpenViewType: " << EditorView::Enum2String(eDefaultOpenViewType) ;

    //Synchronizer settings
    sFtpAddress = value("FtpAddress", "localhost").toString();
    qDebug() << " sync/FtpAddress: " << qPrintable(sFtpAddress);
    sUserName = value("Username", "").toString();
    qDebug() << " sync/Username: " << sUserName;
    sPassword = value("Password", "").toString();
    qDebug() << " sync/Password: " << qPrintable(sPassword);
    sLastFullSynchTime = value("LastFullSynchTime", "").toString();
    qDebug() << " sync/LastFullSynchTime: " << qPrintable(sLastFullSynchTime);
    return true;
}

bool GeneralSettings::save()
{
    //check valid projectname
    Q_ASSERT_X(sCurrentProject.contains(QRegExp("[\n\r ]")) == false, "GeneralSettings::Save", "wrong project name");
    setValue("CurrentProject",sCurrentProject);
    setValue("AutoSaveOnExit",bAutoSaveOnExit);
    setValue("UserDataDir",sUserDataDir);
    setValue("PackingPassword",sPackingPassword);
    setValue("DefaultOpenViewType",EditorView::Enum2String(eDefaultOpenViewType));

    setValue("FtpAddress",sFtpAddress);
    setValue("Username",sUserName);
    setValue("Password",sPassword);
    setValue("LastSynchTime",sLastFullSynchTime);
    return true;
}



Settings::Settings()
{
    General.load();
}

Settings* Settings::instance()
{
    if (m_instancePtr == NULL)
        m_instancePtr = new Settings();
    return m_instancePtr;
}

void Settings::saveSettings()
{
     General.save();
}

