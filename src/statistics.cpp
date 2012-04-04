/**************************************************************************
**
** Author: Miroslav Safr 2012
** Contact: miroslav.safr@gmail.com
**
** Creative Commons — Attribution-NoDerivs 3.0 Unported
** http://creativecommons.org/licenses/by-nd/3.0/legalcode
**
****************************************************************************/

#include <QLabel>
#include <QTimer>
#include <QSplitter>
#include <QDateTime>

#include "statistics.h"
const int iUpdateMs = 500;

StatisticsDock::StatisticsDock(QWidget *parent) :
    QDockWidget(parent)
{
    setWindowTitle(tr("Statistics"));
    QSplitter * mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Vertical);

    QLabel * bootedUpTimeTextLabel = new QLabel("Booted up:",this);
    bootedUpTimeLabel = new QLabel("not available",this);

    QLabel * appRunningTimeTextLabel = new QLabel("Running for:",this);
    appRunningTimeLabel= new QLabel("0 s",this);

    QLabel * lastUpdateTextLabel = new QLabel("Last update:",this);
    lastUpdateLabel= new QLabel(QDateTime::currentDateTime().toString("hh:mm:ss"),this);

    mainSplitter->addWidget(bootedUpTimeTextLabel);
    mainSplitter->addWidget(bootedUpTimeLabel);

    mainSplitter->addWidget(appRunningTimeTextLabel);
    mainSplitter->addWidget(appRunningTimeLabel);


    mainSplitter->addWidget(lastUpdateTextLabel);
    mainSplitter->addWidget(lastUpdateLabel);
    mainSplitter->addWidget(new QWidget());

    setWidget(mainSplitter);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatistics()));
    timer->start(iUpdateMs);
    elapsedTime = new QElapsedTimer();
    elapsedTime->start();
}

void StatisticsDock::updateStatistics()
{
    lastUpdateLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    int iElapsedTime = elapsedTime->elapsed();
    QString sTime("");
    sTime.sprintf("%2.2d:%2.2d:%2.2ds", iElapsedTime / 3600,
           iElapsedTime/60 - ((iElapsedTime / 3600) * 60), iElapsedTime % 60);
    appRunningTimeLabel->setText(sTime);
}

