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

